#ifndef MAIN_H
#define MAIN_H

#include <iomanip>
#include <cstring>
#include <bitset>
#include <iostream>
#include <time.h>
#include "IpPacketGenerator/IpHeader.h"
#include "IpPacketGenerator/RandomIpPacketGenerator.h"
#include "IpPacketGenerator/IpPacket.h"
#include "PDCP/PDCP.hpp"
#include "RLC/rlc_um.h"
#include "MAC/mac_layer.h"

#define SEGMENT_SIZE 3

// IP Packet Generator
std::queue<IpPacket> ip_packet_queue;

// MAC-RLC Layer Queues
std::queue<UmdPdu> rlc_to_mac_queue;
std::queue<UmdPdu> mac_to_rlc_queue;

// MAC-PHY Layer Queues
std::queue<std::vector<MacPDU>> mac_to_phy_queue;
std::queue<std::vector<MacPDU>> phy_to_mac_queue;

// PDCP-RLC Queue
std::queue<std::vector<uint8_t>> pdcp_to_rlc_queue;

// RLC-PDCP Queue
std::queue<std::vector<uint8_t>> rlc_to_pdcp_queue;

// PDCP Configuration and Keys
extern PdcpEntity::PdcpConfig pdcp_config;
extern unsigned char pdcp_cipher_key[32];
extern unsigned char pdcp_integrity_key[16];

// Initialize these in main.cpp
void initializePDCPKeys();

#endif // MAIN_H