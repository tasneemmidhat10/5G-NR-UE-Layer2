CXX = g++
CXXFLAGS = -w -std=c++17 -pthread -I. -I ./rohc-windows/include

LDFLAGS = -L ./rohc-windows/lib
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

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
