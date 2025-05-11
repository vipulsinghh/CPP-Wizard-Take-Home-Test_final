# ABX Exchange Client (C++)

Welcome! This project is a C++ client for the ABX Mock Exchange Server. It was built as a take-home assignment to demonstrate practical skills in networking, binary data handling, and robust software design.

---

## 🌟 What is this project?

This project is a **C++ application** that connects to a mock stock exchange server (the ABX Exchange Server), requests order book data, and saves it as a JSON file. The server simulates real-world stock exchange behavior, including missing data packets, so the client must be smart enough to detect and recover any missing information.

---

## 🛠️ What does the client do?
- **Connects to the ABX server** over TCP (localhost:3000 by default)
- **Requests all available stock ticker packets**
- **Parses binary data** (big-endian format) for each packet (symbol, buy/sell, quantity, price, sequence)
- **Detects missing packets** (by sequence number)
- **Requests any missing packets** from the server
- **Outputs a complete, ordered JSON file** (`ticker_data.json`) with all packets, no missing sequences
- **Handles errors gracefully** (network, parsing, etc.)

---

## 📦 Project Structure

```
final/
├── abx_exchange_server.zip         # Provided server code (unzipped to abx_exchange_server/)
├── abx_exchange_server/            # Server files (after unzipping)
├── include/                       # C++ header files
├── src/                           # C++ implementation files
├── main.cpp                       # Main entry point
├── build/                         # Build output (after compiling)
├── CMakeLists.txt                 # Build configuration
├── README.md                      # This file
└── .gitignore                     # Git ignore rules
```

---

## 🚀 How to Run Everything (Step-by-Step)

### 1. **Clone the repository**
```bash
git clone https://github.com/vipulsinghh/CPP-Wizard-Take-Home-Test_final.git
cd CPP-Wizard-Take-Home-Test_final
```

### 2. **Unzip the server**
```bash
unzip abx_exchange_server.zip -d abx_exchange_server
```

### 3. **Build the C++ client**
```bash
mkdir build
cd build
cmake ..
make
```

### 4. **Start the ABX Exchange Server**
Open a new terminal window/tab and run:
```bash
cd abx_exchange_server
mv main.js main.cjs   # Only needed the first time, to fix Node.js module issues
node main.cjs
```
You should see a message like: `TCP server started on port 3000.`

### 5. **Run the C++ client**
In your build directory (in a separate terminal):
```bash
./abx_client
```

### 6. **Check the output**
A file called `ticker_data.json` will be created in your `build/` directory. This file contains all the order book packets in order, with no missing sequences.

---

## 📝 Example Output
```json
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

---

## 💡 Why is this project interesting?
- **Binary protocol parsing**: Handles raw binary data, not just text!
- **Network resilience**: Detects and recovers missing data automatically.
- **Real-world simulation**: Mimics real stock exchange quirks (like missing packets).
- **Clean, modern C++**: Uses C++17, CMake, and best practices.
- **Easy to run**: No external dependencies except Node.js (for the server) and a C++17 compiler.

---

## 👨‍💻 For the Interviewer
- The code is well-commented and modular for easy review.
- Error handling and edge cases are considered (network, parsing, file I/O).
- The README is written to be beginner-friendly and complete.
- The project demonstrates practical skills in:
  - TCP networking
  - Binary data parsing (big-endian)
  - Robust client-server communication
  - JSON serialization
  - CMake and modern C++
- If you have any questions, please check the code or reach out!

---

## 📋 License
MIT License

---

Thank you for reviewing my project! 🙏

 