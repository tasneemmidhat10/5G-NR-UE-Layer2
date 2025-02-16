#include "RandomIpPacketGenerator.h"
#include <random>

std::random_device rd;  // Only need to call this once
std::mt19937 gen(rd()); // Random number generator based on rd()

uint32_t RandomIpPacketGenerator::generateRandomIp()
{
    std::uniform_int_distribution<> distrib(0, 255);

    uint8_t byte1 = distrib(gen);
    uint8_t byte2 = distrib(gen);
    uint8_t byte3 = distrib(gen);
    uint8_t byte4 = distrib(gen);

    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

uint8_t RandomIpPacketGenerator::generateRandomProtocol()
{
    std::uniform_int_distribution<> distrib(1, 255); // Generic range for protocol

    return static_cast<uint8_t>(distrib(gen));
}

std::vector<uint8_t> RandomIpPacketGenerator::generateRandomPayload(uint16_t length)
{
    std::uniform_int_distribution<> distrib(0, 255);

    std::vector<uint8_t> payload(length);
    for (uint16_t i = 0; i < length; ++i)
    {
        payload[i] = distrib(gen);
    }

    return payload;
}

IpPacket RandomIpPacketGenerator::generateRandomIpPacket(uint16_t payloadLength)
{
    uint32_t srcIp = generateRandomIp();
    uint32_t destIp = generateRandomIp();
    uint8_t protocol = generateRandomProtocol();
    std::vector<uint8_t> payload = generateRandomPayload(payloadLength);

    return IpPacket(srcIp, destIp, payloadLength, protocol, payload);
}
