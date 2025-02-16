# Packet Data Convergance Protocol

## Description

The **Packet Data Convergence Protocol (PDCP)** in the **User Equipment (UE) User Data Plane** is responsible for efficient and secure data transmission in mobile networks (LTE and 5G). It operates between the **RLC (Radio Link Control)** and **IP** layers, optimizing data flow between the UE and the network.

## Basic Functionality

- **Header Compression**: Reduces IP header size for efficient bandwidth usage using ROHC.
- **Ciphering & Integrity Protection**: Ensures data security and confidentiality.
- **In-sequence Delivery & Reordering**: Maintains correct packet order and reorders out-of-sequence packets.
- **Duplicate Detection & Removal**: Eliminates redundant packets to enhance efficiency.

PDCP plays a crucial role in enabling reliable, secure, and optimized data transfer for mobile communication.

## Dependencies

This project uses:

- **[ROHC (Robust Header Compression)](https://rohc-lib.org/)** for header compression.
- **Precompiled OpenSSL for Windows 64-bit** for encryption and security.

## Installation

### Prerequisites

- Windows 64-bit system
- **CMake** and **MSVC (Microsoft Visual Studio)**
- Precompiled OpenSSL binaries (ensure they are in your system path)
- ROHC library installed or built
- g++ compiler

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/Gasimhacker/5G-NR-UE-Layer2.git
   cd PacketDataConverganceProtocol
   ```
2. Build the project (ensure that all the dependancies are intalled and compiled and in the right path)
   **ON WINDOWS use the following command**

```go
g++ -I ./rohc-windows/include -L ./rohc-windows/lib  -o main main.cpp PDCP.cpp ../IpPacketGenerator/IpPacket.cpp ../IpPacketGenerator/IpHeader.cpp ../IpPacketGenerator/RandomIpPacketGenerator.cpp -lrohc -lws2_32 -lssl -lcrypto
```

3. Run the program:
   to run the executable you should use the following command

```go
./main.exe
```

### License

This project is open-source and available under the MIT License
