#ifndef RANDOM_IP_PACKET_GENERATOR_H
#define RANDOM_IP_PACKET_GENERATOR_H

#include "IpPacket.h" // Assuming IpPacket and IpHeader classes are already defined
#include <vector>     // for std::vector
#include <random>     // for random number generation

class RandomIpPacketGenerator
{
private:
    static uint32_t generateRandomIp();
    static uint8_t generateRandomProtocol();
    static std::vector<uint8_t> generateRandomPayload(uint16_t length);

public:
    static IpPacket generateRandomIpPacket(uint16_t payloadLength);
};

#endif // RANDOM_IP_PACKET_GENERATOR_H
