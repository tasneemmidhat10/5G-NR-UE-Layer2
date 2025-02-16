#ifndef UTILS_H
#define UTILS_H

#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

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

struct ProfilingResult
{
    std::string layerName;
    std::string direction;
    double time;
};

void printSummaryTable(const std::vector<ProfilingResult> &results, int segmentSize, int payloadSize, int transportBlockSize)
{
    std::cout << "\n==================== Simulation Summary ====================\n";
    std::cout << "Configuration:\n";
    std::cout << "  Segment Size: " << segmentSize << " bytes\n";
    std::cout << "  Payload Data Size: " << payloadSize << " bytes\n";
    std::cout << "  Transport Block Size: " << transportBlockSize << " bytes\n\n";

    std::cout << "Layer Processing Times:\n";
    std::cout << std::setfill('-') << std::setw(65) << "-" << std::endl;
    std::cout << std::setfill(' ');
    std::cout << std::left << std::setw(20) << "Layer"
              << std::setw(15) << "Direction"
              << std::setw(20) << "Time (ms)" << std::endl;
    std::cout << std::setfill('-') << std::setw(65) << "-" << std::endl;
    std::cout << std::setfill(' ');

    double totalTime = 0;
    double uplinkTotal = 0;
    double downlinkTotal = 0;

    for (const auto &result : results)
    {
        std::cout << std::left << std::setw(20) << result.layerName
                  << std::setw(15) << result.direction
                  << std::fixed << std::setprecision(3) << result.time << std::endl;
        totalTime += result.time;

        if (result.direction == "uplink")
            uplinkTotal += result.time;
        else if (result.direction == "Downlink")
            downlinkTotal += result.time;
    }

    std::cout << std::setfill('-') << std::setw(65) << "-" << std::endl;
    std::cout << std::setfill(' ');
    std::cout << std::left << std::setw(35) << "Uplink Total Time:"
              << std::fixed << std::setprecision(3) << uplinkTotal << " ms" << std::endl;
    std::cout << std::left << std::setw(35) << "Downlink Total Time:"
              << std::fixed << std::setprecision(3) << downlinkTotal << " ms" << std::endl;
    std::cout << std::left << std::setw(35) << "Combined Total Time:"
              << std::fixed << std::setprecision(3) << totalTime << " ms" << std::endl;
    std::cout << std::setfill('-') << std::setw(65) << "-" << std::endl;
}

#endif // UTILS_H