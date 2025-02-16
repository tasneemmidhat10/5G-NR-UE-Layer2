#include "PDCP.hpp"
#include "../IpPacketGenerator/IpHeader.h"
#include "../IpPacketGenerator/RandomIpPacketGenerator.h"
#include "../IpPacketGenerator/IpPacket.h"
#include <rohc/rohc.h>
#include <rohc/rohc_comp.h>
#include <rohc/rohc_decomp.h>
#include <bitset>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iomanip>
#include <cstring>
#include <iostream>

using namespace std;

int main()
{

    PdcpEntity::PdcpConfig config =
        {
            .OutOfOrderDelivery = false,
            .discard_timer = 1000,
            .t_reordering = 500,
            .integrity_protection = true,
            .ciphering = false,
        };

    struct rohc_ts arrival_time = {.sec = 0, .nsec = 0};
    PdcpUplink pdcpUplinkEntity = PdcpUplink(config);
    PdcpDownlink pdcpDownlinkEntity = PdcpDownlink(config);
    rohc_comp *comp = pdcpUplinkEntity.initalizeROHC(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX);
    rohc_decomp *decomp = pdcpDownlinkEntity.initializeRohcDecomp(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX, ROHC_U_MODE);
    pdcpDownlinkEntity.setRX_DELIEV();
    // pdcpUplinkEntity.setSecurityContext();
    unsigned char key[32];
    unsigned char key1[16];
    RAND_bytes(key, sizeof(key));
    RAND_bytes(key1, sizeof(key1));

    for (uint8_t j = 0; j < 5; j++)
    {
        uint16_t length = 18;
        std::vector<uint8_t> serializedPacket;
        IpPacket ippacket = RandomIpPacketGenerator::generateRandomIpPacket(length);
        std::vector<uint8_t> payload = ippacket.getData();
        std::printf("\n");
        for (uint8_t i = 0; i < payload.size(); i++)
        {
            std::printf("0x%02x ", payload[i]);
            if (i != 0 && ((i + 1) % 8 == 0))
            {
                std::printf("\n");
            }
        }
        IpHeader header = ippacket.getHeader();
        header.displayHeader();
        uint32_t srcIp = header.getSourceIp();
        std::bitset<32> binary1(srcIp);
        std::cout << "\n Binary value1 " << binary1 << std::endl;
        std::printf("%d", static_cast<uint8_t>((header.getSourceIp() >> 24) & 0x000000FF));
        std::bitset<8> binary(static_cast<uint8_t>((header.getSourceIp() >> 24) & 0x000000FF));
        std::cout << "\n Binary value " << binary << std::endl;
        // std::cout<<"\n test value "<<x<<std::endl;
        serializedPacket.resize(11); // Ensure enough space for all fields
        serializedPacket[0] = static_cast<uint8_t>((header.getSourceIp() >> 24) & 0xFF);
        serializedPacket[1] = static_cast<uint8_t>((header.getSourceIp() >> 16) & 0xFF);
        serializedPacket[2] = static_cast<uint8_t>((header.getSourceIp() >> 8) & 0xFF);
        serializedPacket[3] = static_cast<uint8_t>((header.getSourceIp() >> 0) & 0xFF);
        serializedPacket[4] = static_cast<uint8_t>((header.getDestIp() >> 24) & 0xFF);
        serializedPacket[5] = static_cast<uint8_t>((header.getDestIp() >> 16) & 0xFF);
        serializedPacket[6] = static_cast<uint8_t>((header.getDestIp() >> 8) & 0xFF);
        serializedPacket[7] = static_cast<uint8_t>((header.getDestIp() >> 0) & 0xFF);
        serializedPacket[8] = header.getProtocol();
        serializedPacket[9] = static_cast<uint8_t>((header.getTotalLength() >> 8) & 0xFF);
        serializedPacket[10] = static_cast<uint8_t>((header.getTotalLength() >> 0) & 0xFF);

        serializedPacket.insert(serializedPacket.end(), payload.begin(), payload.end());
        std::cout << "\n The serialized IP-Packet \n"
                  << std::endl;
        for (uint8_t i = 0; i < serializedPacket.size(); i++)
        {
            std::printf("0x%02x ", serializedPacket[i]);
            if (i != 0 && ((i + 1) % 8 == 0))
            {
                std::printf("\n");
            }
        }
        pdcpUplinkEntity.receive_PDU(serializedPacket);
        std::vector<uint8_t> compreesedPacket = pdcpUplinkEntity.compress_header(arrival_time);
        std::cout << "\n The ROHC compressed IP-Packet \n"
                  << std::endl;
        for (size_t i = 0; i < compreesedPacket.size(); i++)
        {
            std::printf(" 0x%02x", compreesedPacket[i]);
            if (i != 0 && ((i + 1) % 8 == 0))
            {
                std::printf("\n");
            }
        }
        std::vector<uint8_t> integrityCipheredData = PdcpEntity::generateAESCmac(compreesedPacket, key1);
        for (int i = 0; i < 16; i++)
        {
            std::printf(" %02x ", integrityCipheredData[i]);
        }
        std::cout << "\nThe integrityCipheredData size is " << integrityCipheredData.size() << std::endl;
        compreesedPacket.insert(compreesedPacket.end(), integrityCipheredData.begin(), integrityCipheredData.end());
        int packetSize = compreesedPacket.size();
        std::cout << "\nThe packet variable size is " << packetSize << std::endl;
        std::vector<uint8_t> cipheredMessage = pdcpUplinkEntity.pdcpCipher(compreesedPacket, packetSize, key);
        std::cout << "\nThe ciphered message size is " << cipheredMessage.size() << std::endl;
        for (int i = 0; i < cipheredMessage.size(); i++)
        {
            std::printf("0x%02x ", cipheredMessage[i]);
            if (i != 0 && ((i + 1) % 8 == 0))
            {
                std::printf("\n");
            }
        }
        std::vector<uint8_t> pdu = pdcpUplinkEntity.processPDU(cipheredMessage);
        std::vector<uint8_t> SDU = pdcpDownlinkEntity.removePDCPHeader(pdu);
        std::cout << "\n The IP-Packet without the PDCP header \n"
                  << std::endl;
        for (uint8_t i = 0; i < SDU.size(); i++)
        {
            std::printf(" 0x%02x ", SDU[i]);
            if (i != 0 && ((i + 1) % 8 == 0))
            {
                std::printf("\n");
            }
        }
        int packetsize = SDU.size();
        std::vector<uint8_t> decipheredMessage = pdcpDownlinkEntity.pdcpCipherDownlink(SDU, packetsize, key);
        std::cout << "\nThe deciphered message size is " << decipheredMessage.size() << std::endl;
        for (int i = 0; i < decipheredMessage.size(); i++)
        {
            std::printf("0x%02x ", decipheredMessage[i]);
            if (i != 0 && ((i + 1) % 8 == 0))
            {
                std::printf("\n");
            }
        }
        std::vector<uint8_t> integrityVerifiedSDU = pdcpDownlinkEntity.verifyAESCmac(decipheredMessage, key1);
        std::vector<uint8_t> processedSDU = pdcpDownlinkEntity.processSDU(integrityVerifiedSDU);
        pdcpDownlinkEntity.addToRX_BUFFER(processedSDU);
        std::vector<uint8_t> decompressedpacket = pdcpDownlinkEntity.decompressHeader(arrival_time);
        std::cout << "\n The decompressed IP-Packet \n"
                  << std::endl;
        for (uint8_t i = 0; i < decompressedpacket.size(); i++)
        {
            std::printf(" 0x%02x ", decompressedpacket[i]);
            if (i != 0 && ((i + 1) % 8 == 0))
            {
                std::printf("\n");
            }
        }
    }
    return 0;
};
