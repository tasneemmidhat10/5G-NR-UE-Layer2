#include "IpPacket.h"
#include <iostream>
#include <vector>

int main()
{
    uint32_t srcIp = 0xC0A80001;  // 192.168.0.1 (example source IP)
    uint32_t destIp = 0xC0A80002; // 192.168.0.2 (example destination IP)
    uint16_t dataLength = 100;    // Example payload data length (in bytes)
    uint8_t protocol = 6;         // Example protocol (6 for TCP)

    // Sample payload data (random bytes for illustration)
    std::vector<uint8_t> payload(dataLength, 0xAA); // Filling with 0xAA for example

    // Create IP packet
    IpPacket packet(srcIp, destIp, dataLength, protocol, payload);

    // Display the packet details
    packet.displayPacket();

    return 0;
}
