#ifndef PDCP_CONFIG_H
#define PDCP_CONFIG_H

#include <iomanip>
#include <queue>
#include <vector>
#include <string>
#include "PDCP/PDCP.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>

PdcpEntity::PdcpConfig pdcp_config = {
    .OutOfOrderDelivery = false,
    .discard_timer = 1000,
    .t_reordering = 500,
    .integrity_protection = true,
    .ciphering = false,
};
unsigned char pdcp_cipher_key[32];
unsigned char pdcp_integrity_key[16];

void initializePDCPKeys()
{
    RAND_bytes(pdcp_cipher_key, sizeof(pdcp_cipher_key));
    RAND_bytes(pdcp_integrity_key, sizeof(pdcp_integrity_key));
}

#endif // PDCP_CONFIG_H