#include "IpHeader.h"
#include <iostream>

constexpr uint16_t htons(uint16_t hostshort)
{
    return (hostshort >> 8) | (hostshort << 8);
}

IpHeader::IpHeader(uint32_t srcIp, uint32_t destIp, uint16_t dataLength, uint8_t protocol)
{
    versionAndIhl = (4 << 4) | 5; // Version 4, IHL 5 (no options)
    tos = 0;                      // Default ToS
    totalLength = htons(sizeof(IpHeader) + dataLength);
    id = 0;                    // Typically set to 0 or some random value
    flagsAndOffset = 0;        // No fragmentation (0)
    ttl = 64;                  // Default TTL
    this->protocol = protocol; // Set the protocol (e.g., TCP=6, UDP=17)
    checksum = 0;              // Set to 0 initially (to be calculated later)
    this->srcIp = srcIp;       // Set source IP address
    this->destIp = destIp;     // Set destination IP address
}

void IpHeader::displayHeader() const
{
    std::cout << "IP Header Details:" << std::endl;
    std::cout << "Version and IHL: " << (versionAndIhl >> 4) << ", IHL: " << (versionAndIhl & 0xF) << std::endl;
    std::cout << "Type of Service: " << +tos << std::endl;
    std::cout << "Total Length: " << totalLength << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Flags and Fragment Offset: " << flagsAndOffset << std::endl;
    std::cout << "TTL: " << +ttl << std::endl;
    std::cout << "Protocol: " << +protocol << std::endl;
    std::cout << "Checksum: " << checksum << std::endl;
    std::cout << "Source IP: " << srcIp << std::endl;
    std::cout << "Destination IP: " << destIp << std::endl;
}

uint32_t IpHeader::getSourceIp() const { return srcIp; }
uint32_t IpHeader::getDestIp() const { return destIp; }
uint16_t IpHeader::getTotalLength() const { return totalLength; }
uint8_t IpHeader::getProtocol() const { return protocol; }