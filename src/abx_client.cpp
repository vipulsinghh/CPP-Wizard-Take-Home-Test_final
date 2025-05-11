#include "abx_client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <netdb.h>

ABXClient::ABXClient(const std::string& host, int port)
    : host_(host), port_(port), socket_fd_(-1), is_connected_(false) {
    buffer_.reserve(1024);
}

ABXClient::~ABXClient() {
    if (socket_fd_ != -1) {
        close(socket_fd_);
    }
}

bool ABXClient::connect() {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host_.c_str(), std::to_string(port_).c_str(), &hints, &res) != 0) {
        std::cerr << "Failed to get address info" << std::endl;
        return false;
    }

    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd_ == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        freeaddrinfo(res);
        return false;
    }

    if (::connect(socket_fd_, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        close(socket_fd_);
        socket_fd_ = -1;
        freeaddrinfo(res);
        return false;
    }

    freeaddrinfo(res);
    is_connected_ = true;
    return true;
}

bool ABXClient::sendRequest(uint8_t callType, uint8_t resendSeq) {
    if (!is_connected_) {
        std::cerr << "Not connected to server" << std::endl;
        return false;
    }

    uint8_t request[2] = {callType, resendSeq};
    ssize_t sent = send(socket_fd_, request, sizeof(request), 0);
    return sent == sizeof(request);
}

bool ABXClient::requestAllPackets() {
    return sendRequest(1);
}

bool ABXClient::requestPacket(int32_t sequence) {
    return sendRequest(2, static_cast<uint8_t>(sequence));
}

bool ABXClient::receiveData() {
    if (!is_connected_) return false;

    buffer_.clear();
    uint8_t temp_buffer[1024];
    ssize_t bytes_received;

    while ((bytes_received = recv(socket_fd_, temp_buffer, sizeof(temp_buffer), 0)) > 0) {
        buffer_.insert(buffer_.end(), temp_buffer, temp_buffer + bytes_received);
    }

    return !buffer_.empty();
}

ABXClient::Packet ABXClient::parsePacket(const std::vector<uint8_t>& data, size_t& offset) {
    Packet packet;
    packet.symbol = parseString(data, offset, 4);
    packet.buy_sell = static_cast<char>(data[offset++]);
    packet.quantity = parseInt32(data, offset);
    packet.price = parseInt32(data, offset);
    packet.sequence = parseInt32(data, offset);
    convertToHostByteOrder(packet);
    return packet;
}

int32_t ABXClient::parseInt32(const std::vector<uint8_t>& data, size_t& offset) {
    int32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        value = (value << 8) | data[offset++];
    }
    return value;
}

std::string ABXClient::parseString(const std::vector<uint8_t>& data, size_t& offset, size_t length) {
    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        if (data[offset + i] != 0) {
            result += static_cast<char>(data[offset + i]);
        }
    }
    offset += length;
    return result;
}

void ABXClient::convertToHostByteOrder(Packet& packet) {
    packet.quantity = ntohl(packet.quantity);
    packet.price = ntohl(packet.price);
    packet.sequence = ntohl(packet.sequence);
}

void ABXClient::processData() {
    size_t offset = 0;
    while (offset + PACKET_SIZE <= buffer_.size()) {
        Packet packet = parsePacket(buffer_, offset);
        std::lock_guard<std::mutex> lock(packets_mutex_);
        packets_.push_back(packet);
    }
}

void ABXClient::handleMissingSequences() {
    if (packets_.empty()) return;

    std::sort(packets_.begin(), packets_.end(),
              [](const Packet& a, const Packet& b) { return a.sequence < b.sequence; });

    int32_t expected_seq = 1;
    for (const auto& packet : packets_) {
        while (expected_seq < packet.sequence) {
            std::cout << "Requesting missing sequence: " << expected_seq << std::endl;
            if (!requestPacket(expected_seq)) {
                std::cerr << "Failed to request missing sequence: " << expected_seq << std::endl;
                return;
            }
            if (!receiveData()) {
                std::cerr << "Failed to receive missing sequence: " << expected_seq << std::endl;
                return;
            }
            processData();
            expected_seq++;
        }
        expected_seq = packet.sequence + 1;
    }
}

bool ABXClient::saveToJson(const std::string& filename) {
    std::lock_guard<std::mutex> lock(packets_mutex_);
    std::sort(packets_.begin(), packets_.end(),
              [](const Packet& a, const Packet& b) { return a.sequence < b.sequence; });

    nlohmann::json j = nlohmann::json::array();
    for (const auto& packet : packets_) {
        j.push_back({
            {"symbol", packet.symbol},
            {"buy_sell", std::string(1, packet.buy_sell)},
            {"quantity", packet.quantity},
            {"price", packet.price},
            {"sequence", packet.sequence}
        });
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    file << j.dump(4);
    return true;
} 