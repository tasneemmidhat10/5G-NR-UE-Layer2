#ifndef IP_HEADER_H
#define IP_HEADER_H

// for uint8_t, uint16_t, uint32_t types
#include <cstdint>

class IpHeader {
private:
    uint8_t versionAndIhl;    // Version (4 bits) and IHL (4 bits)
    uint8_t tos;              // Type of Service
    uint16_t totalLength;     // Total length of the IP packet (header + data)
    uint16_t id;              // Identification field for fragmentation
    uint16_t flagsAndOffset;  // Flags (3 bits) and Fragment Offset (13 bits)
    uint8_t ttl;              // Time to Live (maximum number of hops)
    uint8_t protocol;         // Protocol used in the payload (e.g., TCP = 6, UDP = 17)
    uint16_t checksum;        // Header checksum for error checking
    uint32_t srcIp;           // Source IP address
    uint32_t destIp;          // Destination IP address

public:
    IpHeader(uint32_t srcIp, uint32_t destIp, uint16_t dataLength, uint8_t protocol);
    void displayHeader() const;

    // Getters for header fields
    uint32_t getSourceIp() const;
    uint32_t getDestIp() const;
    uint16_t getTotalLength() const;
    uint8_t getProtocol() const;
};

#endif // IP_HEADER_H
