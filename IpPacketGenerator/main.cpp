#include "RandomIpPacketGenerator.h"
#include <iostream>

int main()
{
    // Generate a random IP packet with 100 bytes of payload data
    IpPacket randomPacket = RandomIpPacketGenerator::generateRandomIpPacket(100);
    IpPacket randomPacket2 = RandomIpPacketGenerator::generateRandomIpPacket(100);
    IpPacket randomPacket3 = RandomIpPacketGenerator::generateRandomIpPacket(100);
    IpPacket randomPacket4 = RandomIpPacketGenerator::generateRandomIpPacket(100);

    // Display the details of the generated packet
    randomPacket.displayPacket();
    randomPacket2.displayPacket();
    randomPacket3.displayPacket();
    randomPacket4.displayPacket();

    return 0;
}