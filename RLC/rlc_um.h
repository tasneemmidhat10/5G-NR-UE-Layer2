#ifndef RLC_UM_H
#define RLC_UM_H

#include <vector>
#include <cstdint>
#include <map>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>

// Define RLC SDU as a byte array
using RlcSdu = std::vector<uint8_t>;

// Define UMD PDU structure
struct UmdPdu {
    uint8_t si : 2;  // Segmentation Info (2 bits)
    uint8_t reserved : 6;  // Reserved bits (unused)
    uint16_t sn;  // Sequence Number (6 or 12 bits)
    uint16_t so;  // Segment Offset (16 bits, optional)
    std::vector<uint8_t> data;  // Payload (RLC SDU or segment)
};

// Transmitting UM RLC Entity
class TransmittingUmRlcEntity {
public:
    TransmittingUmRlcEntity();
    void transmitSdu(const RlcSdu& sdu, size_t segmentSize);

private:
    struct UmRlcState {
        uint16_t txNext = 0;  // Next SN to assign for transmission
    } state;

    std::vector<UmdPdu> generateUmdPdus(const RlcSdu& sdu, uint16_t sn, size_t segmentSize);
    void submitToLowerLayer(const UmdPdu& pdu);
};

// Receiving UM RLC Entity
class ReceivingUmRlcEntity {
public:
    ReceivingUmRlcEntity();
    ~ReceivingUmRlcEntity();
    void receivePdu(const UmdPdu& pdu);

private:
    struct SegmentInfo {
        std::map<uint16_t, std::vector<uint8_t>> segments;  // SO -> data
        size_t expected_size = 0;
    };

    struct UmRxState {
        uint16_t rx_next_reassembly = 0;
        uint16_t rx_next_highest = 0;
        uint16_t rx_timer_trigger = 0;
        const uint16_t um_window_size = 32;  // For 6-bit SN
        std::map<uint16_t, SegmentInfo> reassembly_buffer;
    } state;

    std::atomic<bool> running;
    std::thread timer_thread;
    std::chrono::milliseconds t_reassembly_duration = std::chrono::milliseconds(100);  // Example: 100ms
    std::chrono::steady_clock::time_point t_reassembly_start;
    bool t_reassembly_running = false;

    void timerLoop();
    void checkTReassemblyExpiry();
    void handleTReassemblyExpiry();
    void deliverToUpperLayer(const RlcSdu& sdu);
    void updateReassemblyWindow();
    void startTReassembly();
    void stopTReassembly();
    bool isInsideReassemblyWindow(uint16_t sn) const;
    void checkCompleteSdu(uint16_t sn);
    void manageReassemblyTimer();
    void advanceRxNextReassembly();
};

#endif // RLC_UM_H
