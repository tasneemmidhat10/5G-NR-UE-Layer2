# RLC Layer Simulation

This directory contains an implementation of the 5G NR RLC (Radio Link Control) Layer. It includes transmitting and receiving entities and provides a profiling tool to measure performance.

## Compilation and Usage

### Prerequisites
Ensure you have the following installed:
- `g++` (supporting C++17)
- `make`

### Compilation
To compile the project, run:
```sh
make
```
This will generate:
- `rlc_sim`: The main simulation executable
- `profiler`: The profiling tool
- `librlc.a`: A static library containing the compiled transmitting and receiving logic

### Running the Simulation
To run the main simulation:
```sh
./rlc_sim
```

To run the profiler:
```sh
./profiler
```

### Cleaning Up
To remove compiled files and executables:
```sh
make clean
```

## File Structure
- `main.cpp` - Entry point for the simulation.
- `profiler.cpp` - Measures the performance of RLC layer functions.
- `transmitting.cpp` - Handles RLC transmitting logic.
- `receiving.cpp` - Handles RLC receiving logic.
- `rlc_um.h` - The only header file, which includes the necessary headers for both transmitting and receiving logic.
- `Makefile` - Build script for compiling the project.

## Notes
- The project uses multithreading with `std::thread` for handling timed events in receiving.
- A static library (`librlc.a`) is created to avoid recompiling common components each time.
- The current implementation only includes the UM (Unacknowledged Mode) entity.
- Future improvements include adding support for the AM (Acknowledged Mode) entity.

