#include "mac_layer.h"
#include "../RLC/rlc_um.h"

MAC::MAC(std::queue<UmdPdu> *rlc_queue,
         std::queue<std::vector<MacPDU>> *phy_queue)
    : rlc_queue_(rlc_queue), phy_queue_(phy_queue) {}

void MAC::print_status(const std::string &message) const
{
    std::cout << "[MAC Layer] " << message << std::endl;
}

void MAC::print_hex_data(const MacPDU &macPdu) const
{
    std::cout << "Hex Data (Length: 0x"
              << std::hex << std::setw(4) << std::setfill('0')
              << macPdu.length << ", Data: ";

    // Print the data payload in hex format
    const uint8_t *data = reinterpret_cast<const uint8_t *>(&macPdu.data);
    for (size_t i = 0; i < macPdu.length; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(data[i]) << " ";
    }

    std::cout << ")" << std::dec << std::endl;
}

void MAC::mac_multiplexing()
{
    print_status("Starting MAC Multiplexing...");

    while (!rlc_queue_->empty())
    {
        std::vector<MacPDU> transportBlock;
        MacPDU mac_pdu;

        size_t remaining = TRANSPORT_BLOCK_SIZE; // Now using global variable

        while (!rlc_queue_->empty() && remaining > 2)
        {
            auto &rlc_pdu = rlc_queue_->front();

            uint16_t length = static_cast<uint16_t>(sizeof(rlc_pdu));
            size_t needed = 2 + length; // 2-byte header

            if (needed > remaining)
                break;

            // Add MAC subheader (big-endian)
            mac_pdu.length = length;
            mac_pdu.data = rlc_pdu;

            // transportBlock.push_back(static_cast<uint8_t>((length >> 8) & 0xFF));
            // transportBlock.push_back(static_cast<uint8_t>(length & 0xFF));

            // Add RLC PDU payload
            // transportBlock.insert(transportBlock.end(), mac_pdu.begin(), mac_pdu.end());
            transportBlock.push_back(mac_pdu);

            remaining -= needed;
            rlc_queue_->pop();
        }

        if (!transportBlock.empty())
        {
            phy_queue_->push(transportBlock);
            print_status("Created MAC PDU:");

            for (auto &mac_pdu : transportBlock)
            {
                print_hex_data(mac_pdu);
            }
        }
    }

    print_status("Multiplexing completed successfully\n");
}

void MAC::mac_demultiplexing()
{
    print_status("Starting MAC Demultiplexing...");

    while (!phy_queue_->empty())
    {
        auto trasportBlock = phy_queue_->front();
        phy_queue_->pop();
        size_t index = 0;

        print_status("Processing MAC PDU:");
        // print_hex_data(trasportBlock);

        for (auto &mac_pdu : trasportBlock)
        {
            // // Extract length from header
            // uint16_t length = mac_pdu.length;
            // index += 2;

            // std::cout << "Length: " << length << std::endl;
            // if (length == 0 || (index + length) > trasportBlock.size())
            //     break;

            // std::cout << "Index: " << index << std::endl;

            rlc_queue_->push(mac_pdu.data);
            // index += length;

            print_status("Extracted RLC PDU:");
            print_hex_data(mac_pdu);
        }

        // while (index + 2 <= trasportBlock.size())
        // {
        //     // Extract length from header
        //     // uint16_t length = (static_cast<uint16_t>(mac_pdu[index]) << 8) |
        //     //                   mac_pdu[index + 1];

        //     uint16_t length = trasportBlock[index].length;
        //     index += 2;

        //     if (length == 0 || (index + length) > trasportBlock.size())
        //         break;

        //     // Extract RLC PDU
        //     std::vector<uint8_t> rlc_pdu(
        //         mac_pdu.begin() + index,
        //         mac_pdu.begin() + index + length);

        //     rlc_queue_->push(rlc_pdu);
        //     index += length;

        //     print_status("Extracted RLC PDU:");
        //     print_hex_data(rlc_pdu);
        // }
    }

    print_status("Demultiplexing completed successfully\n");
}