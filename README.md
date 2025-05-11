#ABX Exchange Client (C++)

#Project Overview

This project is a C++ client application designed to interact with the ABX Mock Exchange Server. The client connects to the server over TCP, requests order book data, handles missing data packets, and outputs the complete data as a JSON file. The project demonstrates skills in network programming, binary data parsing, error handling, and robust client-server communication.

## Code Structure

- `main.cpp`: Entry point for the client application. Handles the overall workflow: connecting to the server, requesting data, processing responses, handling missing sequences, and saving the output.
- `include/abx_client.h`: Header file for the ABXClient class, which encapsulates all networking and data processing logic.
- `src/abx_client.cpp`: Implementation of the ABXClient class. Contains methods for connecting to the server, sending requests, receiving and parsing binary data, detecting and recovering missing packets, and writing the final JSON output.
- `CMakeLists.txt`: Build configuration file for CMake. Sets up dependencies and build rules.
- `abx_exchange_server.zip`: Provided server code. Unzip this to get the server files.
- `abx_exchange_server/`: Directory containing the unzipped server files, including the server entry point (`main.js` or `main.cjs`).
- `build/`: Directory where the compiled client binary and output files are placed.

## How the Code Works

1. **Connection:** The client connects to the ABX server at `localhost:3000` using TCP.
2. **Initial Data Request:** The client sends a request to stream all available packets. The server responds with a series of binary packets, each containing order book data (symbol, buy/sell indicator, quantity, price, sequence number).
3. **Parsing:** The client parses each binary packet, converting fields from big-endian format to host format, and stores them in memory.
4. **Detecting Missing Sequences:** After receiving all packets, the client checks for missing sequence numbers in the data. If any are missing, it sends a request to the server to resend each missing packet.
5. **Recovery:** The client receives and processes any resent packets, ensuring that the final data set is complete and ordered by sequence.
6. **Output:** The client writes all packets to a JSON file (`ticker_data.json`), with each object representing a packet and all sequences present.

## How to Build and Run

### Prerequisites
- C++17 compatible compiler
- CMake 3.10 or higher
- Node.js 16.17.0 or higher (for the server)
- Git

### Step-by-Step Instructions

1. **Clone the repository:**
   ```bash
   git clone https://github.com/vipulsinghh/CPP-Wizard-Take-Home-Test_final.git
   cd CPP-Wizard-Take-Home-Test_final
   ```

2. **Unzip the server files:**
   ```bash
   unzip abx_exchange_server.zip -d abx_exchange_server
   ```

3. **Build the C++ client:**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Start the ABX Exchange Server:**
   Open a new terminal window and run:
   ```bash
   cd abx_exchange_server
   mv main.js main.cjs   # Only needed the first time, to fix Node.js module issues
   node main.cjs
   ```
   You should see a message indicating the server is running on port 3000.

5. **Run the C++ client:**
   In your build directory (in a separate terminal):
   ```bash
   ./abx_client
   ```

6. **Check the output:**
   After running, a file called `ticker_data.json` will be created in your `build/` directory. This file contains all the order book packets in order, with no missing sequences.

## Example Output

The output JSON file will look like this:

```
[
  {
    "symbol": "MSFT",
    "buy_sell": "B",
    "quantity": 50,
    "price": 100,
    "sequence": 1
  },
  ...
]
```

## Notes for Reviewers
- The code is modular and well-commented for clarity.
- All error cases (network, parsing, file I/O) are handled gracefully.
- The client ensures that no sequence is missing in the final output, as required by the specification.
- The README provides all necessary steps to build and run the project from scratch.
