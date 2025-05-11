#pragma once

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <queue>
#include <atomic>
#include <nlohmann/json.hpp>

class ABXClient {
public:
    struct Packet {
        std::string symbol;
        char buy_sell;
        int32_t quantity;
        int32_t price;
        int32_t sequence;
    };

    ABXClient(const std::string& host = "127.0.0.1", int port = 3000);
    ~ABXClient();

    // Connect to the server
    bool connect();
    
    // Request all packets
    bool requestAllPackets();
    
    // Request a specific packet
    bool requestPacket(int32_t sequence);
    
    // Process received data
    void processData();
    
    // Save data to JSON file
    bool saveToJson(const std::string& filename = "ticker_data.json");

private:
    // Socket operations
    bool sendRequest(uint8_t callType, uint8_t resendSeq = 0);
    bool receiveData();
    
    // Data parsing
    Packet parsePacket(const std::vector<uint8_t>& data, size_t& offset);
    int32_t parseInt32(const std::vector<uint8_t>& data, size_t& offset);
    std::string parseString(const std::vector<uint8_t>& data, size_t& offset, size_t length);
    
    // Helper functions
    void handleMissingSequences();
    void convertToHostByteOrder(Packet& packet);
    
    // Member variables
    std::string host_;
    int port_;
    int socket_fd_;
    std::vector<Packet> packets_;
    std::mutex packets_mutex_;
    std::atomic<bool> is_connected_;
    std::vector<uint8_t> buffer_;
    static constexpr size_t PACKET_SIZE = 17; // 4 + 1 + 4 + 4 + 4 bytes
}; 