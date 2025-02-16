# Compiler settings
CXX = g++
CXXFLAGS = -I ./rohc-windows/include -w

# Libraries
LIBS = -L ./rohc-windows/lib -lrohc -lws2_32 -lssl -lcrypto

# Source files
SRCS = main.cpp \
       PDCP.cpp \
       ../IpPacketGenerator/IpPacket.cpp \
       ../IpPacketGenerator/IpHeader.cpp \
       ../IpPacketGenerator/RandomIpPacketGenerator.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = main.exe

# Default target
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

# Compilation rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	del $(TARGET) $(OBJS)

.PHONY: all clean
