#ifndef MAC_LAYER_H
#define MAC_LAYER_H

#include <queue>
#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include "../RLC/rlc_um.h"

struct MacPDU
{
    uint16_t length; // Length of the MAC PDU
    UmdPdu data;     // Payload (RLC SDU or segment)
};

class MAC
{
public:
    // Transport Block Size configuration (adjust in header)
    static constexpr size_t TRANSPORT_BLOCK_SIZE = 1024; // 32 bytes example

    // Initialize with input and output queues
    MAC(std::queue<UmdPdu> *rlc_queue,
        std::queue<std::vector<MacPDU>> *phy_queue);

    // Transmit path functions
    void mac_multiplexing();

    // Receive path functions
    void mac_demultiplexing();

private:
    // Queue pointers for layer communication
    std::queue<UmdPdu> *rlc_queue_;
    std::queue<std::vector<MacPDU>> *phy_queue_;

    // Helper functions
    void print_hex_data(const MacPDU &data) const;
    void print_status(const std::string &message) const;
};

#endif // MAC_LAYER_H