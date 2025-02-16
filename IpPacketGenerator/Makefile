# Compiler
CXX = g++.exe
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

# Source files and target
SRC = main.cpp RandomIpPacketGenerator.cpp IpHeader.cpp IpPacket.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = main.exe

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
	del /F /Q $(OBJ)  # Clean up object files after linking

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	del /F /Q $(TARGET) $(OBJ)
