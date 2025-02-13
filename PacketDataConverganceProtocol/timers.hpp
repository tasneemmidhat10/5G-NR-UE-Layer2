#include <iostream>
#include <queue>
#include <unordered_map>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>

#define DISCARD_TIMER_MS 5000
std::mutex tx_mutex;

// Map to track active discard timers
std::unordered_map<int, std::future<void>> discard_timers;

// Function to remove PDU from TX_BUFFER based on SN
void remove_pdu(int SN);

// Discard timer function (runs asynchronously)
void start_discard_timer(int SN);

// Function to acknowledge a PDU before its timer expires (simulate successful transmission)
void acknowledge_pdu(int SN);
// Function to add PDU to TX_BUFFER and start discard timer
void add_pdu_to_tx_buffer(int SN, std::string data);