#ifndef IP_PACKET_H
#define IP_PACKET_H

#include "IpHeader.h"
#include <vector>  // for handling dynamic payload data

class IpPacket {
private:
    IpHeader header;           // IP header
    std::vector<uint8_t> data; // Payload data

public:
    IpPacket(uint32_t srcIp, uint32_t destIp, uint16_t dataLength, uint8_t protocol, const std::vector<uint8_t>& payload);
    void displayPacket() const;

    IpHeader getHeader() const;
    std::vector<uint8_t> getData() const;
};

#endif // IP_PACKET_H
