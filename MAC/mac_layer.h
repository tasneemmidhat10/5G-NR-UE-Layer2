#ifndef MAC_LAYER_H
#define MAC_LAYER_H

#include <queue>
#include <vector>
#include <iostream>
#include <iomanip>
#include "mac_pdu.h"
#include "../main.h"

class MAC
{
public:
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