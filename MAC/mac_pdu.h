#ifndef MAC_PDU_H
#define MAC_PDU_H

#include <cstdint>
#include "../RLC/rlc_um.h"

struct MacPDU
{
    uint16_t length; // Length of the MAC PDU
    UmdPdu data;     // Payload (RLC SDU or segment)
};

#endif // MAC_PDU_H
