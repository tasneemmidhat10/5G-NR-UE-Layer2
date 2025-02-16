#include "main.h"
#include "utils.h"
#include "pdcp_config.h"
#include "Profiler/profiler.h"

#include <iomanip>
#include <queue>
#include <vector>
#include <string>

void ipPacketGenerator()
{
    std::cout << "\n IpPacketGenerator UPLINK START =========================================== \n"
              << std::endl;

    uint16_t length = 18;
    // Generate a random IP packet with 18 bytes of payload data
    IpPacket ippacket = RandomIpPacketGenerator::generateRandomIpPacket(18);
    ip_packet_queue.push(ippacket);

    // Display the details of the generated packet
    ippacket.displayPacket();
}

void PDCPLayerUplink()
{
    if (ip_packet_queue.empty())
    {
        std::cout << "No IP packets to process\n";
        return;
    }
    std::cout << "\n PDCP UPLINK START =========================================== \n"
              << std::endl;

    IpPacket ippacket = ip_packet_queue.front();

    struct rohc_ts arrival_time = {.sec = 0, .nsec = 0};
    PdcpUplink pdcpUplinkEntity = PdcpUplink(pdcp_config);
    PdcpDownlink pdcpDownlinkEntity = PdcpDownlink(pdcp_config);
    rohc_comp *comp = pdcpUplinkEntity.initalizeROHC(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX);
    rohc_decomp *decomp = pdcpDownlinkEntity.initializeRohcDecomp(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX, ROHC_U_MODE);
    pdcpDownlinkEntity.setRX_DELIEV();

    std::vector<uint8_t> serializedPacket;
    std::vector<uint8_t> payload = ippacket.getData();

    IpHeader header = ippacket.getHeader();
    uint32_t srcIp = header.getSourceIp();
    std::bitset<32> binary1(srcIp);
    std::cout << "\n Binary value1 " << binary1 << std::endl;
    std::printf("%d", static_cast<uint8_t>((header.getSourceIp() >> 24) & 0x000000FF));
    std::bitset<8> binary(static_cast<uint8_t>((header.getSourceIp() >> 24) & 0x000000FF));
    std::cout << "\n Binary value " << binary << std::endl;

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
    std::vector<uint8_t> integrityCipheredData = PdcpEntity::generateAESCmac(compreesedPacket, pdcp_integrity_key);
    for (int i = 0; i < 16; i++)
    {
        std::printf(" %02x ", integrityCipheredData[i]);
    }
    std::cout << "\nThe integrityCipheredData size is " << integrityCipheredData.size() << std::endl;
    compreesedPacket.insert(compreesedPacket.end(), integrityCipheredData.begin(), integrityCipheredData.end());
    int packetSize = compreesedPacket.size();
    std::cout << "\nThe packet variable size is " << packetSize << std::endl;
    std::vector<uint8_t> cipheredMessage = pdcpUplinkEntity.pdcpCipher(compreesedPacket, packetSize, pdcp_cipher_key);
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
    pdcp_to_rlc_queue.push(pdu);
}

void RLCLayerUplink()
{
    if (pdcp_to_rlc_queue.empty())
    {
        std::cout << "No PDCP PDUs to process\n";
        return;
    }

    std::cout << "\n RLC UPLINK START =========================================== \n"
              << std::endl;

    std::vector<uint8_t> pdu = pdcp_to_rlc_queue.front();

    TransmittingUmRlcEntity transmitter;
    std::vector<UmdPdu> umdPdus = transmitter.transmitSdu(pdu, SEGMENT_SIZE);

    std::cout << umdPdus.size() << " UMD PDUs generated\n";

    for (const auto &pdu : umdPdus)
    {
        if (pdu.si != 0b00)
        {
            std::cout << "Transmitting PDU: SN=" << pdu.sn << ", SO=" << pdu.so << ", SI=" << static_cast<int>(pdu.si) << "\n";
            rlc_to_mac_queue.push(pdu);
        }
    }
}

void macLayerUplink()
{
    if (rlc_to_mac_queue.empty())
    {
        std::cout << "No RLC PDUs to process\n";
        return;
    }

    std::cout << "\n MAC UPLINK START =========================================== \n"
              << std::endl;

    // Initialize MAC transmitter
    MAC mac_tx(&rlc_to_mac_queue, &mac_to_phy_queue);
    print_queue(rlc_to_mac_queue, "Original RLC");
    mac_tx.mac_multiplexing();
    print_queue(mac_to_phy_queue, "MAC to PHY");
}

void transportBlockLooping()
{
    if (mac_to_phy_queue.empty())
    {
        std::cout << "No MAC PDUs to process\n";
        return;
    }

    std::cout << "\n Transport Block Looping START =========================================== \n"
              << std::endl;

    while (!mac_to_phy_queue.empty())
    {
        if (!SimulateLoss())
        {
            phy_to_mac_queue.push(mac_to_phy_queue.front());
        }
        else
        {
            std::cout << "Transport Block dropped!" << std::endl;
        }

        mac_to_phy_queue.pop();
    }
}

void MACLayerDownlink()
{

    if (phy_to_mac_queue.empty())
    {
        std::cout << "No MAC PDUs to process\n";
        return;
    }

    // // Initialize MAC receiver
    MAC mac_rx(&mac_to_rlc_queue, &phy_to_mac_queue);

    std::cout << "\n MAC DOWNLINK START =========================================== \n"
              << std::endl;

    std::cout << "\n=== Downlink Test (Demultiplexing) ===\n";
    print_queue(phy_to_mac_queue, "PHY to MAC");
    mac_rx.mac_demultiplexing();
    print_queue(mac_to_rlc_queue, "MAC to RLC");
}

void RLCLayerDownlink()
{
    if (mac_to_rlc_queue.empty())
    {
        std::cout << "No MAC PDUs to process\n";
        return;
    }

    std::cout << "\n RLC DOWNLINK START =========================================== \n"
              << std::endl;

    ReceivingUmRlcEntity receiver;

    while (!mac_to_rlc_queue.empty())
    {
        UmdPdu data = mac_to_rlc_queue.front();
        RlcSdu sdu = receiver.receivePdu(data);
        if (!sdu.empty())
        {
            rlc_to_pdcp_queue.push(sdu);
        }
        mac_to_rlc_queue.pop();
    }
}

void PDCPLayerDownlink()
{
    if (rlc_to_pdcp_queue.empty())
    {
        std::cout << "No RLC SDUs to process\n";
        return;
    }

    std::cout << "\n PDCP DOWNLINK START =========================================== \n"
              << std::endl;

    struct rohc_ts arrival_time = {.sec = 0, .nsec = 0};
    PdcpDownlink pdcpDownlinkEntity = PdcpDownlink(pdcp_config);
    rohc_decomp *decomp = pdcpDownlinkEntity.initializeRohcDecomp(ROHC_SMALL_CID, ROHC_SMALL_CID_MAX, ROHC_U_MODE);
    pdcpDownlinkEntity.setRX_DELIEV();

    std::vector<uint8_t> pdu = rlc_to_pdcp_queue.front();

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
    std::vector<uint8_t> decipheredMessage = pdcpDownlinkEntity.pdcpCipherDownlink(SDU, packetsize, pdcp_cipher_key);
    std::cout << "\nThe deciphered message size is " << decipheredMessage.size() << std::endl;
    for (int i = 0; i < decipheredMessage.size(); i++)
    {
        std::printf("0x%02x ", decipheredMessage[i]);
        if (i != 0 && ((i + 1) % 8 == 0))
        {
            std::printf("\n");
        }
    }
    std::vector<uint8_t> integrityVerifiedSDU = pdcpDownlinkEntity.verifyAESCmac(decipheredMessage, pdcp_integrity_key);
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

void profileLayer(void (*layer)(), std::string layerName, std::string direction)
{
    Profiler profiler;

    profiler.start();
    layer();
    profiler.stop();
    std::cout << "\n\n======== " << layerName << " took " << profiler.average_time() << " milliseconds for " << direction << " ========\n\n";
    profiler.reset();
}

void downLink()
{
    Profiler profiler;
    profiler.start();

    profileLayer(MACLayerDownlink, "MACLayer", "Downlink");
    profileLayer(RLCLayerDownlink, "RLCLayer", "Downlink");
    profileLayer(PDCPLayerDownlink, "MACLayer", "Downlink");

    profiler.stop();
    std::cout << "\n\n======== Downlink took " << profiler.average_time() << " milliseconds ========\n\n";
}


void upLink()
{
    Profiler profiler;
    profiler.start();

    profileLayer(ipPacketGenerator, "ipPacketGenerator", "uplink");
    profileLayer(PDCPLayerUplink, "PDCPLayer", "uplink");
    profileLayer(RLCLayerUplink, "RLCLayer", "uplink");
    profileLayer(macLayerUplink, "macLayer", "uplink");

    profiler.stop();
    std::cout << "\n======== Uplink took " << profiler.average_time() << " milliseconds ========\n";
}

int main()
{
    std::cout << "\n ================= Starting simulation... =================" << std::endl;
    initializePDCPKeys();
    std::cout << "\n ================ Starting uplink simulation... ================" << std::endl;
    upLink();
    std::cout << "\n ================ Starting downlink simulation... ================" << std::endl;
    transportBlockLooping();
    downLink();

    std::cout << "\n ================= Simulation complete! =================" << std::endl;

    return 0;
}