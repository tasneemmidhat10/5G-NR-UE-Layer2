#ifndef UTILS_H
#define UTILS_H

#include <queue>
#include <vector>
#include <string>

#define LOSS_PROBABILITY 0.01

template <typename T>
void print_queue(const std::queue<T> &q, const std::string &name)
{
    std::cout << "\n"
              << name << " Queue (" << q.size() << " PDUs):\n";
    std::queue<T> temp = q;
    int count = 1;

    while (!temp.empty())
    {
        std::cout << "PDU " << count++ << ": ";
        const auto &pdu = temp.front();

        if constexpr (std::is_same_v<T, std::vector<uint8_t>>)
        {
            for (const auto &byte : pdu)
            {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << "0x" << static_cast<int>(byte) << " ";
            }
        }
        else if constexpr (std::is_same_v<T, std::vector<MacPDU>>)
        {
            std::cout << "Transport Block with " << pdu.size() << " MAC PDUs:\n";
            for (const auto &mac_pdu : pdu)
            {
                std::cout << "  MAC PDU length: " << mac_pdu.length
                          << ", RLC PDU SN: " << mac_pdu.data.sn
                          << ", SI: " << static_cast<int>(mac_pdu.data.si) << "\n";
            }
        }
        else
        {
            std::cout << "Size: " << sizeof(pdu);
        }

        std::cout << std::dec << "\n";
        temp.pop();
    }
    std::cout << std::endl;
}

bool SimulateLoss()
{
    srand(time(0));
    float randomValue = (float)rand() / RAND_MAX;
    return randomValue < LOSS_PROBABILITY;
}

#endif // UTILS_H