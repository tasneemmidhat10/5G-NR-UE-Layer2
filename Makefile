CXX = g++
CXXFLAGS = -w -std=c++17 -pthread -I. -I ./rohc-windows/include -I ./OpenSSL-Win64/include

LDFLAGS = -L ./rohc-windows/lib -L ./OpenSSL-Win64/lib
LIBS = -lrohc -lws2_32 -lssl -lcrypto

TARGET = main
SRCS = main.cpp \
       PDCP/PDCP.cpp \
       IpPacketGenerator/IpHeader.cpp \
       IpPacketGenerator/IpPacket.cpp \
       IpPacketGenerator/RandomIpPacketGenerator.cpp \
	   RLC/receiving.cpp \
	   RLC/transmitting.cpp \
	   MAC/mac_layer.cpp 


OBJS = $(SRCS:.cpp=.o)

# Default values for arguments
SEGMENT_SIZE ?= 3
PAYLOAD_DATA_SIZE ?= 18
TRANSPORT_BLOCK_SIZE ?= 1024

$(TARGET): clean $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	del /Q /F main.exe main.o PDCP\PDCP.o IpPacketGenerator\IpHeader.o IpPacketGenerator\IpPacket.o IpPacketGenerator\RandomIpPacketGenerator.o RLC\receiving.o RLC\transmitting.o MAC\mac_layer.o 2>nul || exit 0

run: $(TARGET)
	./$(TARGET) --segment_size=$(SEGMENT_SIZE) --payload_data_size=$(PAYLOAD_DATA_SIZE) --transport_block_size=$(TRANSPORT_BLOCK_SIZE)
