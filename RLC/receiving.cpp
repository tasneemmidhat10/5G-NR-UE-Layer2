#include "rlc_um.h"

// Receiving UM RLC Entity
ReceivingUmRlcEntity::ReceivingUmRlcEntity() : running(true)
{
    timer_thread = std::thread(&ReceivingUmRlcEntity::timerLoop, this);
}

ReceivingUmRlcEntity::~ReceivingUmRlcEntity()
{
    running = false;
    if (timer_thread.joinable())
    {
        timer_thread.join();
    }
}

void ReceivingUmRlcEntity::receivePdu(const UmdPdu& pdu)
{
    const uint16_t sn = pdu.sn;

    if (pdu.si == 0b00)
    {  // Complete SDU
        deliverToUpperLayer(pdu.data);
        return;
    }

    if (!isInsideReassemblyWindow(sn))
    {
        if (sn >= state.rx_next_highest)
	{
            state.rx_next_highest = sn + 1;
	    updateReassemblyWindow();
        }
        return;
    }

    if (sn < state.rx_next_reassembly)
    {
        return;
    }

    auto& seg_info = state.reassembly_buffer[sn];
    seg_info.segments[pdu.so] = pdu.data;

    // Update expected size if this is the last segment
    if (pdu.si == 0b10) 
    {  // Last segment
        seg_info.expected_size = pdu.so + pdu.data.size();
    }

    checkCompleteSdu(sn);
    manageReassemblyTimer();
}

void ReceivingUmRlcEntity::timerLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Check every 10ms
        checkTReassemblyExpiry();
    }
}

void ReceivingUmRlcEntity::checkTReassemblyExpiry() {
    if (t_reassembly_running && 
        std::chrono::steady_clock::now() - t_reassembly_start >= t_reassembly_duration) {
        handleTReassemblyExpiry();
    }
}

void ReceivingUmRlcEntity::handleTReassemblyExpiry() {
    state.rx_next_reassembly = state.rx_timer_trigger;
    advanceRxNextReassembly();
    updateReassemblyWindow();
    manageReassemblyTimer();
}

bool ReceivingUmRlcEntity::isInsideReassemblyWindow(uint16_t sn) const {
    const uint16_t window_start = (state.rx_next_highest + 64 - state.um_window_size) % 64;
    const uint16_t window_end = state.rx_next_highest;

    if (window_start < window_end) {
        return sn >= window_start && sn < window_end;
    } else {
        return sn >= window_start || sn < window_end;
    }
}

void ReceivingUmRlcEntity::checkCompleteSdu(uint16_t sn) {
    auto& seg_info = state.reassembly_buffer[sn];
    if (seg_info.expected_size == 0) return;

    size_t total_size = 0;
    for (const auto& [so, data] : seg_info.segments) {
        total_size += data.size();
    }

    if (total_size >= seg_info.expected_size) {
        RlcSdu sdu;
        for (const auto& [so, data] : seg_info.segments) {
            sdu.insert(sdu.end(), data.begin(), data.end());
        }
        deliverToUpperLayer(sdu);
        state.reassembly_buffer.erase(sn);

        if (sn == state.rx_next_reassembly) {
            advanceRxNextReassembly();
        }
    }
}

void ReceivingUmRlcEntity::advanceRxNextReassembly() {
    while (state.reassembly_buffer.count(state.rx_next_reassembly)) {
        state.rx_next_reassembly = (state.rx_next_reassembly + 1) % 64;
    }
}

void ReceivingUmRlcEntity::manageReassemblyTimer() {
    bool needs_timer = (state.rx_next_highest > state.rx_next_reassembly + 1) ||
                       (state.rx_next_highest == state.rx_next_reassembly + 1 &&
                        state.reassembly_buffer.count(state.rx_next_reassembly));

    if (needs_timer && !t_reassembly_running) {
        startTReassembly();
        state.rx_timer_trigger = state.rx_next_highest;
    } else if (!needs_timer && t_reassembly_running) {
        stopTReassembly();
    }
}

void ReceivingUmRlcEntity::updateReassemblyWindow() {
    const uint16_t window_start = (state.rx_next_highest + 64 - state.um_window_size) % 64;
    auto it = state.reassembly_buffer.begin();
    while (it != state.reassembly_buffer.end()) {
        if (!isInsideReassemblyWindow(it->first)) {
            it = state.reassembly_buffer.erase(it);
        } else {
            ++it;
        }
    }
}

void ReceivingUmRlcEntity::deliverToUpperLayer(const RlcSdu& sdu) {
    std::cout << "Delivered SDU size: " << sdu.size() << " bytes\n";
}

void ReceivingUmRlcEntity::startTReassembly() {
    t_reassembly_start = std::chrono::steady_clock::now();
    t_reassembly_running = true;
    std::cout << "t-Reassembly started\n";
}

void ReceivingUmRlcEntity::stopTReassembly() {
    t_reassembly_running = false;
    std::cout << "t-Reassembly stopped\n";
}
