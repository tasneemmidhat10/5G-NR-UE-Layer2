#include "rlc_um.h"
#include <thread>
#include <chrono>

int main()
{
    TransmittingUmRlcEntity transmitter;
    ReceivingUmRlcEntity receiver;

    // Example RLC SDU
    RlcSdu sdu = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    // Transmit the SDU with a segment size of 3 bytes
    transmitter.transmitSdu(sdu, 3);

    // Simulate receiving PDUs
    UmdPdu pdu1;
    pdu1.si = 0b01; // First segment
    pdu1.reserved = 0;
    pdu1.sn = 0;
    pdu1.so = 0;
    pdu1.data = {0x01, 0x02, 0x03};

    UmdPdu pdu2;
    pdu2.si = 0b10; // Last segment
    pdu2.reserved = 0;
    pdu2.sn = 0;
    pdu2.so = 3;
    pdu2.data = {0x04, 0x05, 0x06, 0x07, 0x08};

    receiver.receivePdu(pdu1);
    receiver.receivePdu(pdu2); // This should trigger SDU delivery

    // Simulate timer expiry
    std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Wait for timer to expire

    return 0;
}
