#ifndef MAIN_H
#define MAIN_H

#include <iomanip>
#include <cstring>
#include <bitset>
#include <iostream>
#include <time.h>
#include <queue>
#include <vector>
#include "MAC/mac_pdu.h" // Add this include
#include "IpPacketGenerator/IpHeader.h"
#include "IpPacketGenerator/RandomIpPacketGenerator.h"
#include "IpPacketGenerator/IpPacket.h"
#include "PDCP/PDCP.hpp"
#include "RLC/rlc_um.h"
#include "MAC/mac_layer.h"

extern int SEGMENT_SIZE;
extern int PAYLOAD_DATA_SIZE;
extern int TRANSPORT_BLOCK_SIZE; // Add this line

// PDCP Configuration and Keys
extern PdcpEntity::PdcpConfig pdcp_config;
extern unsigned char pdcp_cipher_key[32];
extern unsigned char pdcp_integrity_key[16];

// Initialize these in main.cpp
void initializePDCPKeys();

#endif // MAIN_H