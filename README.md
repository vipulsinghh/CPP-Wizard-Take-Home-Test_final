# ABX Exchange Client

This is a C++ client application that interacts with the ABX exchange server to retrieve and process stock ticker data.

## Prerequisites

- C++17 or higher
- CMake 3.10 or higher
- Node.js 16.17.0 or higher (for running the server)
- Git

## Project Structure

```
.
├── CMakeLists.txt
├── include/
│   └── abx_client.h
├── src/
│   └── abx_client.cpp
├── main.cpp
├── abx_exchange_server.zip
└── README.md
```

## Setup Instructions

1. Clone the repository:
```bash
git clone <your-repo-url>
cd <repo-name>
```

2. Extract the server files:
```bash
unzip abx_exchange_server.zip -d abx_exchange_server
```

3. Build the C++ client:
```bash
mkdir build
cd build
cmake ..
make
```

## Running the Application

1. Start the ABX exchange server:
```bash
cd abx_exchange_server
node main.js
```

2. In a new terminal, run the C++ client:
```bash
cd build
./abx_client
```

The client will:
- Connect to the server at localhost:3000
- Request and receive stock ticker data
- Handle any missing sequences
- Generate a JSON file containing all packets in sequence

## Output

The client generates a `ticker_data.json` file containing an array of objects, where each object represents a packet with the following structure:
```json
{
    "symbol": "MSFT",
    "buy_sell": "B",
    "quantity": 100,
    "price": 150,
    "sequence": 1
}
```

## Error Handling

The client includes robust error handling for:
- Network connection issues
- Data parsing errors
- Missing sequences
- Server disconnections

## Notes

- The server must be running before starting the client
- The client will automatically request any missing sequences
- All data is processed in big-endian format as per specification
- The connection is automatically closed after receiving all data

## License

MIT License 