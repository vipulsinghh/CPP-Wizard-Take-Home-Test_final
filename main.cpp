#include "abx_client.h"
#include <iostream>
#include <csignal>

volatile sig_atomic_t g_running = 1;

void signal_handler(int) {
    g_running = 0;
}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    ABXClient client;
    
    std::cout << "Connecting to ABX exchange server..." << std::endl;
    if (!client.connect()) {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    std::cout << "Requesting all packets..." << std::endl;
    if (!client.requestAllPackets()) {
        std::cerr << "Failed to request packets" << std::endl;
        return 1;
    }

    std::cout << "Receiving data..." << std::endl;
    if (!client.receiveData()) {
        std::cerr << "Failed to receive data" << std::endl;
        return 1;
    }

    std::cout << "Processing received data..." << std::endl;
    client.processData();

    std::cout << "Checking for missing sequences..." << std::endl;
    client.handleMissingSequences();

    std::cout << "Saving data to JSON file..." << std::endl;
    if (!client.saveToJson()) {
        std::cerr << "Failed to save data to JSON file" << std::endl;
        return 1;
    }

    std::cout << "Data successfully saved to ticker_data.json" << std::endl;
    return 0;
} 