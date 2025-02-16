#include "IpPacket.h"
#include <iostream>
#include <algorithm> // for std::min

IpPacket::IpPacket(uint32_t srcIp, uint32_t destIp, uint16_t dataLength, uint8_t protocol, const std::vector<uint8_t> &payload)
    : header(srcIp, destIp, dataLength, protocol), data(payload) {}

void IpPacket::displayPacket() const
{
    header.displayHeader();
    std::cout << "Payload (Data) Length: " << data.size() << " bytes" << std::endl;
    std::cout << "Payload Data (first 16 bytes): ";
    for (size_t i = 0; i < std::min(data.size(), size_t(16)); ++i)
    {
        std::cout << std::hex << +data[i] << " ";
    }
    std::cout << std::dec << std::endl; // Switch back to decimal for readability
}

IpHeader IpPacket::getHeader() const { return header; }
std::vector<uint8_t> IpPacket::getData() const { return data; }
