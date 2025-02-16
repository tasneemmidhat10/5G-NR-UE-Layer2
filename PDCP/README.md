# PDCP Layer Implementation

## Prerequisites

- Windows operating system
- C++ compiler (g++)
- Make build system
- OpenSSL library
- ROHC library

## Project Structure

```
.
├── bin/            - Binary files
├── rohc-windows/   - ROHC library files
├── share/          - Shared resources
├── Ciphering.cpp   - Ciphering implementation
├── compressor.cpp  - Header compression
├── PDCP.cpp        - Core PDCP functionality
├── PDCP.hpp        - PDCP header file
└── timers.cpp      - Timer implementations
```

## Installation Guide

### 1. Setting up the Build Environment

Follow the main project readme for setting up Chocolatey and Make.

### 2. Installing Required Libraries

Follow OpenSSL and ROHC setup instructions in the main project readme.

### 3. Building the Project

1. Navigate to PDCP directory:

   ```bash
   cd PDCP
   ```

2. Build commands:
   ```bash
   make        # Build the project
   ./main      # Run the application
   make clean  # Clean build files
   ```

## Troubleshooting

See the main project readme for common issues and solutions.
