```go 
g++ -I ./rohc-windows/include -L ./rohc-windows/lib -o main main.cpp PDCP.cpp ./5G-NR-UE-Layer2/dummyIPPacketGenerator/IpPacket.cpp ./5G-NR-UE-Layer2/dummyIPPacketGenerator/IpHeader.cpp ./5G-NR-UE-Layer2/dummyIPPacketGenerator/RandomIpPacketGenerator.cpp -lrohc -lws2_32 
./main.exe 
```