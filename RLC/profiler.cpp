#include "rlc_um.h"
#include <thread>
#include <chrono>
#include <vector>
#include "../Profiler/profiler.h"

void profileTransmission(TransmittingUmRlcEntity& transmitter, const RlcSdu& sdu, size_t segmentSize) {
    Profiler profiler;
    profiler.start();

    transmitter.transmitSdu(sdu, segmentSize);

    profiler.stop();
    std::cout << "Transmission profiling (SDU size: " << sdu.size() << ", segment size: " << segmentSize << "): "
              << profiler.average_time() << " microseconds\n";
}

void profileReception(ReceivingUmRlcEntity& receiver, const std::vector<UmdPdu>& pdus) {
    Profiler profiler;
    profiler.start();

    for (const auto& pdu : pdus) {
        receiver.receivePdu(pdu);
    }

    profiler.stop();
    std::cout << "Reception profiling (PDU count: " << pdus.size() << "): "
              << profiler.average_time() << " microseconds\n";
}

int main() {
    TransmittingUmRlcEntity transmitter;
    ReceivingUmRlcEntity receiver;

    // Test with varying SDU sizes and segment sizes
    std::vector<size_t> sdu_sizes = {64, 128, 256, 512};  // Bytes
    std::vector<size_t> segment_sizes = {32, 64, 128};    // Bytes

    for (size_t sdu_size : sdu_sizes) {
        RlcSdu sdu(sdu_size, 0xAA);  // Create an SDU filled with 0xAA

        for (size_t segment_size : segment_sizes) {
            std::cout << "Profiling SDU size: " << sdu_size << ", segment size: " << segment_size << "\n";

            // Profile transmission
            profileTransmission(transmitter, sdu, segment_size);

            // Simulate receiving PDUs
            std::vector<UmdPdu> pdus;
            for (uint16_t offset = 0; offset < sdu_size; offset += segment_size) {
                UmdPdu pdu{
                    .si = (offset == 0) ? (uint8_t)0b01 : (offset + segment_size >= sdu_size) ? (uint8_t)0b10 : (uint8_t)0b11,
		    .reserved = 0,
                    .sn = 0,
                    .so = (offset),
                    .data = std::vector<uint8_t>(
                        sdu.begin() + offset,
                        sdu.begin() + std::min(offset + segment_size, sdu_size)
                    )
                };
                pdus.push_back(pdu);
            }

            // Profile reception
            profileReception(receiver, pdus);

            std::cout << "----------------------------------------\n";
        }
    }

    return 0;
}
