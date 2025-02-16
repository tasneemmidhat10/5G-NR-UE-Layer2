# RLC Layer Implementation

## Prerequisites

- Windows operating system
- C++ compiler (g++)
- Make build system

## Project Structure

```
.
├── librlc.a        - Static library containing core RLC functionality
├── main.cpp        - Entry point for the simulation
├── Makefile        - Build configuration
├── profiler.cpp    - Performance measurement tool
├── README.md       - This documentation
├── receiving.cpp   - RLC receiving entity implementation
├── rlc_um.h       - Main header file for RLC UM mode
└── transmitting.cpp- RLC transmitting entity implementation
```

## Installation Guide

### 1. Setting up the Build Environment

Follow the main project readme for setting up Chocolatey and Make.

### 2. Building the Project

1. Navigate to RLC directory:

   ```bash
   cd RLC
   ```

2. Build commands:
   ```bash
   make        # Build the project
   ./main      # Run the application
   make clean  # Clean build files
   ```

## Troubleshooting

See the main project readme for common issues and solutions.
