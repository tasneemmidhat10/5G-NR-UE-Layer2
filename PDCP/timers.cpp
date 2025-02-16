#include "timers.hpp"

// Function to remove PDU from TX_BUFFER based on SN
void remove_pdu(int SN) {
    std::lock_guard<std::mutex> lock(tx_mutex);

    std::queue<PDCP_PDU> temp_queue;
    bool removed = false;

    while (!TX_BUFFER.empty()) {
        PDCP_PDU pdu = TX_BUFFER.front();
        TX_BUFFER.pop();

        if (pdu.SN == SN) {
            removed = true; // Mark as deleted
        } else {
            temp_queue.push(pdu);
        }
    }

    TX_BUFFER = std::move(temp_queue);

    if (removed) {
        std::cout << "[Timer Expired] PDU with SN " << SN << " discarded!\n";
    }
}
// Discard timer function (runs asynchronously)
void start_discard_timer(int SN) {
    discard_timers[SN] = std::async(std::launch::async, [SN]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(DISCARD_TIMER_MS));
        remove_pdu(SN);
    });
}
// Function to acknowledge a PDU before its timer expires (simulate successful transmission)
void acknowledge_pdu(int SN) {
    std::lock_guard<std::mutex> lock(tx_mutex);

    if (discard_timers.find(SN) != discard_timers.end()) {
        std::cout << "[ACK] PDU with SN " << SN << " transmitted, canceling discard timer\n";
        discard_timers.erase(SN); // Stop tracking this timer
    }

    remove_pdu(SN);
}
// Function to add PDU to TX_BUFFER and start discard timer
void add_pdu_to_tx_buffer(int SN, std::string data) {
    std::lock_guard<std::mutex> lock(tx_mutex);
    
    PDCP_PDU new_pdu = {SN, data};
    TX_BUFFER.push(new_pdu);

    std::cout << "[TX_BUFFER] Added PDU with SN " << SN << "\n";

    // Start discard timer
    start_discard_timer(SN);
}