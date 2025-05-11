ABX Exchange Client (C++)

Welcome to the ABX Exchange Client project. This C++ application was developed as part of a take-home assessment to showcase skills in socket programming, binary data processing, and resilient software architecture. The client communicates with a simulated stock exchange server and ensures complete and accurate data retrieval under imperfect network conditions.

Project Overview

This project implements a C++ client that connects to the ABX Mock Exchange Server, retrieves real-time order book packets over TCP, detects and recovers from any missing data, and outputs a clean, sequential JSON file. The client has been designed to handle binary data efficiently and recover gracefully from data loss or transmission issues.

Client Functionality

Establishes a TCP connection with the ABX server running on localhost port 3000

Requests all available order book data packets from the server

Parses each packet's binary payload in big-endian format (fields include symbol, buy/sell indicator, quantity, price, and sequence number)

Tracks sequence numbers to identify and request missing packets

Aggregates and stores the reconstructed dataset in an ordered JSON file named ticker_data.json

Handles network and parsing errors with robust error-checking mechanisms

Project Structure

final/
├── abx_exchange_server.zip - Provided mock server code archive
├── abx_exchange_server/ - Extracted server files
├── include/ - C++ header files
├── src/ - Source implementation files
├── main.cpp - Application entry point
├── build/ - Output directory for compiled binaries
├── CMakeLists.txt - CMake build configuration
├── README.md - Project documentation
└── .gitignore - Files to be ignored by Git

Setup and Execution Guide

Clone the repository

git clone https://github.com/vipulsinghh/CPP-Wizard-Take-Home-Test_final.git
cd CPP-Wizard-Take-Home-Test_final

Extract the server archive

unzip abx_exchange_server.zip -d abx_exchange_server

Build the C++ client

mkdir build
cd build
cmake ..
make

Start the ABX Exchange Server

Open a new terminal window and execute the following:

cd abx_exchange_server
mv main.js main.cjs (Run this once to ensure compatibility with Node.js)
node main.cjs

You should see: TCP server started on port 3000

Run the client

In the build directory (in a separate terminal):

./abx_client

Verify the output

Upon successful execution, a file named ticker_data.json will be created in the build directory. It will contain a complete set of order book entries sorted by sequence number.

Sample Output

[
{
"symbol": "MSFT",
"buy_sell": "B",
"quantity": 50,
"price": 100,
"sequence": 1
},
{
"symbol": "AAPL",
"buy_sell": "S",
"quantity": 20,
"price": 110,
"sequence": 2
}
...
]

Closing Remarks

This project demonstrates a practical application of low-level networking concepts combined with data consistency strategies. It simulates a real-world scenario where reliability and robustness in data handling are critical. Thank you for taking the time to review this work.

