# IP Packet Generator

## Prerequisites

- Windows operating system
- C++ compiler (g++)
- Make build system
- WinSock2 library (ws2_32)

## Project Structure

```
.
├── IpHeader.cpp     - IP header implementation
├── IpHeader.h       - IP header structure definition
├── IpPacket.cpp     - IP packet implementation
├── IpPacket.h       - IP packet structure definition
├── main.cpp         - Program entry point
├── Makefile        - Build configuration
├── RandomIpPacketGenerator.cpp  - Generator implementation
└── RandomIpPacketGenerator.h    - Generator interface
```

## Installation Guide

### 1. Setting up the Build Environment

Follow the main project readme for setting up Chocolatey and Make.

### 2. Building the Project

1. Navigate to IpPacketGenerator directory:

   ```bash
   cd IpPacketGenerator
   ```

2. Build commands:
   ```bash
   make              # Build the project
   ./main            # Run the application
   make clean        # Clean build files
   ```

## Troubleshooting

See the main project readme for common issues and solutions.
