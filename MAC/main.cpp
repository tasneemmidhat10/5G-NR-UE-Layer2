#include "mac_layer.h"
// #include <queue>

// void print_queue(std::queue<std::vector<uint8_t>>& q, const std::string& name) {
//     std::cout << "\n" << name << " Queue (" << q.size() << " PDUs):\n";
//     std::queue<std::vector<uint8_t>> temp = q;
//     int count = 1;

//     while (!temp.empty()) {
//         std::cout << "PDU " << count++ << ": ";
//         auto pdu = temp.front();
//         for (auto byte : pdu) {
//             std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
//                       << static_cast<int>(byte) << " ";
//         }
//         std::cout << std::dec << "\n";
//         temp.pop();
//     }
// }

int main()
{
    // // Create queues for uplink path
    // std::queue<std::vector<uint8_t>> rlc_to_mac, mac_to_phy;

    // // Initialize MAC transmitter
    // MAC mac_tx(&rlc_to_mac, &mac_to_phy);

    // // Create test RLC PDUs (hex format)
    // rlc_to_mac.push({0x01, 0x02, 0x03});       // 3 bytes
    // rlc_to_mac.push({0x04, 0x05, 0x06, 0x07}); // 4 bytes
    // rlc_to_mac.push({0x08, 0x09});             // 2 bytes

    // std::cout << "=== Uplink Test (Multiplexing) ===\n";
    // print_queue(rlc_to_mac, "Original RLC");
    // mac_tx.mac_multiplexing();
    // print_queue(mac_to_phy, "MAC to PHY");

    // // Create queues for downlink path
    // std::queue<std::vector<uint8_t>> phy_to_mac, mac_to_rlc;

    // // Initialize MAC receiver
    // MAC mac_rx(&mac_to_rlc, &phy_to_mac);

    // // Move MAC PDUs to receiver input
    // while (!mac_to_phy.empty()) {
    //     phy_to_mac.push(mac_to_phy.front());
    //     mac_to_phy.pop();
    // }

    // std::cout << "\n=== Downlink Test (Demultiplexing) ===\n";
    // print_queue(phy_to_mac, "PHY to MAC");
    // mac_rx.mac_demultiplexing();
    // print_queue(mac_to_rlc, "MAC to RLC");

    return 0;
}