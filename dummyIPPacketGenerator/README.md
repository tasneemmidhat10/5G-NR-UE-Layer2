# Random IP Packet Generator

This project generates random IP packets by randomly creating IP headers (source IP, destination IP, protocol) and generating random payload data. The random IP packets are then displayed in a human-readable format.

## Features
- Generates random source and destination IP addresses (IPv4).
- Randomly selects a protocol (e.g., TCP, UDP).
- Generates random payload data of specified length.
- Displays the details of the generated packet (IP header and payload data).

## Files
- **`IpHeader.h`**: Defines the IP header structure.
- **`IpHeader.cpp`**: Implements the methods for the IP header.
- **`IpPacket.h`**: Defines the IP packet structure that includes the header and payload.
- **`IpPacket.cpp`**: Implements the methods for handling the IP packet.
- **`RandomIpPacketGenerator.h`**: Defines the class that generates random IP packets.
- **`RandomIpPacketGenerator.cpp`**: Implements the methods for generating random IP packets.
- **`main.cpp`**: The entry point of the program that generates and displays a random IP packet.

## Prerequisites

To run this project, you need to have a C++ compiler installed. You can use:

- **Linux/MacOS**: `g++`
- **Windows**: MinGW or Visual Studio

### For Linux/MacOS

You will need the following packages installed:
- `g++` (C++ compiler)

### For Windows

You can use **MinGW** or **Visual Studio** to compile and run the project.

## Getting Started

### 1. Clone the repository
If you haven't already cloned the repository, do so by running:

```bash
git clone https://github.com/Gasimhacker/5G-NR-UE-Layer2.git
```

### 2. Build the Project

Navigate to the directory where your files are stored, and use the following commands to compile the project.
```go
cd 5G-NR-UE-Layer2/dummyIPPacketGenerator
```
#### On Linux/MacOS (using `g++`):

1. Open a terminal in the project directory.
2. Run the following command to compile the project:

   ```bash
   g++ -o randomIpPacketGenerator main.cpp RandomIpPacketGenerator.cpp IpHeader.cpp IpPacket.cpp
   ```

#### On Windows (using MinGW):

1. Open Command Prompt or PowerShell in the project directory.
2. Run the following command:

   ```bash
   g++ -o randomIpPacketGenerator.exe main.cpp RandomIpPacketGenerator.cpp IpHeader.cpp IpPacket.cpp
   ```

   For windows users add -lws2_32 flag 
   
   ```bash
   g++ -o randomIpPacketGenerator main.cpp RandomIpPacketGenerator.cpp IpHeader.cpp IpPacket.cpp -lws2_32
   ```

   If you're using **Visual Studio**, you'll need to create a new project and add all the `.cpp` and `.h` files to the project, then build the solution.

### 3. Run the Program

#### On Linux/MacOS:

Once the code is compiled, you can run the program with the following command:

```bash
./randomIpPacketGenerator
```

#### On Windows:

On Windows (using MinGW), run:

```bash
randomIpPacketGenerator.exe
```

### 4. Output

The program will generate a random IP packet with the specified payload length and print its details, including the source IP, destination IP, protocol, and the payload data. The output will look something like this:

```
IP Header Details:
Version and IHL: 4, IHL: 5
Type of Service: 0
Total Length: 116
ID: 0
Flags and Fragment Offset: 0
TTL: 64
Protocol: 183
Checksum: 0
Source IP: 308295036
Destination IP: 106526072
Payload (Data) Length: 100 bytes
Payload Data (first 16 bytes): d2 74 96 b3 76 f5 f2 d1 a2 c7 d0 fd fd 39 21 a4
```

## Customization

You can modify the number of bytes in the payload by passing a different value when generating the random IP packet. For example, you can pass `payloadLength = 50` to generate a packet with a 50-byte payload.

The protocol field is randomly generated between values 1 and 255. If you want to limit it to specific protocols like TCP (6) or UDP (17), you can modify the `generateRandomProtocol` function in `RandomIpPacketGenerator.cpp`.

## Troubleshooting

- **Error: "htons is undefined"**:
    - This error occurs if you’re missing the proper header for `htons`. On Windows, include `<winsock2.h>` at the top of your `IpHeader.cpp`. If you're on Linux, use `<arpa/inet.h>`.
  
- **Compiler errors related to missing files**:
    - Ensure that all `.cpp` and `.h` files are in the same directory and correctly referenced in your project.

## License

This project is open-source and available under the [MIT License](LICENSE).

---