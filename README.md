# 5G Layer 2 Software Implementation

This project implements Layer 2 functionalities of a 5G NR UE (User Equipment) including PDCP, RLC, and MAC layers.

## Prerequisites

- Windows operating system
- C++ compiler (g++)
- Make build system
- OpenSSL library
- ROHC (RObust Header Compression) library

## Project Structure

```
.
├── IpPacketGenerator/
│   ├── IpHeader.cpp
│   ├── IpHeader.h
│   ├── IpPacket.cpp
│   ├── IpPacket.h
│   ├── main.cpp
│   ├── Makefile
│   ├── RandomIpPacketGenerator.cpp
│   ├── RandomIpPacketGenerator.h
│   └── README.md
├── MAC/
│   ├── mac_layer.cpp
│   ├── mac_layer.h
│   ├── main.cpp
│   └── Makefile
├── OpenSSL-Win64/
├── PDCP/
│   ├── bin/
│   ├── rohc-windows/
│   ├── share/
│   ├── Author.md
│   ├── Ciphering.cpp
│   ├── compressor.cpp
│   ├── PDCP.cpp
│   ├── PDCP.hpp
│   ├── rohc_init_params.hpp
│   ├── timers.cpp
│   ├── timers.hpp
│   └── Makefile
├── Profiler/
│   ├── profiler.h
│   └── README.md
├── RLC/
│   ├── main.cpp
│   ├── Makefile
│   ├── profiler.cpp
│   ├── receiving.cpp
│   ├── rlc_um.h
│   └── transmitting.cpp
├── rohc-windows/
├── AUTHORS
├── main.cpp
├── main.h
├── Makefile
├── pdcp_config.h
├── README.md
├── rlc.lib
└── utils.h
```

## Installation Guide

### 1. Setting up the Build Environment

#### Installing Chocolatey (Package Manager)

1. Open PowerShell as Administrator
2. Run the installation command:
   ```powershell
   Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
   ```
3. Verify installation: `choco --version`

#### Installing Make

1. Install via Chocolatey: `choco install make`
2. Verify installation: `make --version`

### 2. Installing Required Libraries

#### OpenSSL Setup

1. Download [Win64 OpenSSL v3.4.1](https://slproweb.com/download/Win64OpenSSL-3_4_1.exe)
2. Run the installer
3. Create an 'OpenSSL-Win64' directory in the project root
4. Copy the include and lib directories from the installation to the project's OpenSSL-Win64 directory
5. Verify installation: `openssl version`

#### ROHC Library

- The ROHC library is pre-included in the `rohc-windows` directory
- No additional installation required

### 3. Building the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/Gasimhacker/5G-NR-UE-Layer2.git
   ```

2. Navigate to project directory:

   ```bash
   cd 5G-NR-UE-Layer2
   ```

3. Build commands:

   ```bash
   make        # Build the project
   make run    # Run the application
   ```

4. Running the application:

   ```bash
   # Run with default values (SEGMENT_SIZE=3, PAYLOAD_DATA_SIZE=18, TRANSPORT_BLOCK_SIZE=1024)
   make run

   # Run with custom segment size
   make run SEGMENT_SIZE=5

   # Run with custom payload data size
   make run PAYLOAD_DATA_SIZE=30

   # Run with custom transport block size
   make run TRANSPORT_BLOCK_SIZE=2048

   # Run with multiple custom values
   make run SEGMENT_SIZE=5 PAYLOAD_DATA_SIZE=30 TRANSPORT_BLOCK_SIZE=2048

   # Or run directly with the executable
   ./main --segment_size=5 --payload_data_size=30 --transport_block_size=2048
   ```

   Available arguments:

   - `--segment_size`: Sets the RLC segmentation size (default: 3)
   - `--payload_data_size`: Sets the IP packet payload size in bytes (default: 18)
   - `--transport_block_size`: Sets the MAC transport block size in bytes (default: 1024)

### 4. Running Individual Layers

Each layer can be built and run separately using their respective Makefiles:

#### IP Packet Generator

```bash
cd IpPacketGenerator
make
./main
```

#### PDCP Layer

```bash
cd PDCP
make
./main
```

#### RLC Layer

```bash
cd RLC
make
./main        # Run main program
```

#### MAC Layer

```bash
cd MAC
make
./main
```

## Troubleshooting

### Common Issues and Solutions

1. TLS/SSL errors during Chocolatey installation:

   ```powershell
   [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
   ```

2. Make not found in PATH:

   - Close and reopen PowerShell, or
   - Add to PATH manually:
     ```powershell
     $env:Path += ";C:\ProgramData\chocolatey\bin"
     ```

3. OpenSSL linking issues:

   - Ensure OpenSSL-Win64 directory exists in project root
   - Verify include and lib directories are properly copied
   - Check if CXXFLAGS and LDFLAGS in Makefile point to correct paths

4. ROHC-related issues:
   - For missing DLLs: Copy from rohc-windows/bin to project directory
   - For linker errors: Verify LDFLAGS and LIBS in Makefile include -lrohc

## Debugging Library Issues

### OpenSSL Troubleshooting

1. Missing OpenSSL DLLs:

   ```powershell
   # Check if OpenSSL DLLs are in system PATH
   where libssl-3-x64.dll
   where libcrypto-3-x64.dll
   ```

   If not found:

   - Copy `libssl-3-x64.dll` and `libcrypto-3-x64.dll` from `C:\Program Files\OpenSSL-Win64\bin` to your project's root directory
   - Or add OpenSSL bin directory to system PATH:
     ```powershell
     $env:Path += ";C:\Program Files\OpenSSL-Win64\bin"
     ```

2. Linker errors (LNK2019, LNK2001):

   - Verify library paths in Makefile:
     ```makefile
     OPENSSL_DIR = ./OpenSSL-Win64
     CXXFLAGS += -I$(OPENSSL_DIR)/include
     LDFLAGS += -L$(OPENSSL_DIR)/lib
     LIBS += -lssl -lcrypto
     ```
   - Check if library files exist:
     ```powershell
     ls ./OpenSSL-Win64/lib/libssl.lib
     ls ./OpenSSL-Win64/lib/libcrypto.lib
     ```

3. Runtime errors:
   ```cpp
   // Add debug prints in your code
   if (!SSL_library_init()) {
       fprintf(stderr, "SSL_library_init failed\n");
       return -1;
   }
   ```

### ROHC Library Troubleshooting

1. Missing ROHC DLLs:

   ```powershell
   # Verify ROHC DLL location
   where librohc.dll
   where librohc_comp.dll
   where librohc_decomp.dll
   ```

   If missing:

   ```powershell
   copy .\rohc-windows\bin\*.dll .
   ```

2. Compilation errors:

   - Check include paths:
     ```makefile
     ROHC_DIR = ./rohc-windows
     CXXFLAGS += -I$(ROHC_DIR)/include
     LDFLAGS += -L$(ROHC_DIR)/lib
     LIBS += -lrohc
     ```
   - Verify header files:
     ```powershell
     ls ./rohc-windows/include/rohc.h
     ls ./rohc-windows/include/rohc_comp.h
     ```

3. Library load errors:
   ```cpp
   // Add debug code
   #include <dlfcn.h>
   void *handle = dlopen("librohc.dll", RTLD_LAZY);
   if (!handle) {
       fprintf(stderr, "ROHC load error: %s\n", dlerror());
       return -1;
   }
   ```

### Using Dependencies Checker Script

Create a file `check_deps.ps1`:

```powershell
Write-Host "Checking OpenSSL..."
$openssl = Get-Command openssl -ErrorAction SilentlyContinue
if ($openssl) {
    Write-Host "OpenSSL found: $($openssl.Version)"
} else {
    Write-Host "OpenSSL not found!" -ForegroundColor Red
}

Write-Host "`nChecking library files..."
$files = @(
    ".\OpenSSL-Win64\lib\libssl.lib",
    ".\OpenSSL-Win64\lib\libcrypto.lib",
    ".\rohc-windows\lib\librohc.lib"
)
foreach ($file in $files) {
    if (Test-Path $file) {
        Write-Host "Found: $file"
    } else {
        Write-Host "Missing: $file" -ForegroundColor Red
    }
}
```

Run the script:

```powershell
.\check_deps.ps1
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Authors

- Huda Omer <hudaoe175@gmail.com>
- Monzer Omer <monzeromer048@gmail.com>
- Tasneem Midhat <tasneemmidhat@gmail.com>
- Mohamed Ahmed <mohamedahmedgasim2000@gmail.com>
- Mugtba Mirghani Mohamed <mmmhasy@gmail.com>
- Ahmed AlSiddig <ahmedalaqib@gmail.com>
- Ayman Tarig <aymantarig17@gmail.com>
- Mohammed Asharf <mhmdashraf0159@gmail.com>
