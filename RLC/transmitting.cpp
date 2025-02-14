#include "rlc_um.h"

// Constructor
TransmittingUmRlcEntity::TransmittingUmRlcEntity() : state{0} {}

// Function to transmit an RLC SDU
void TransmittingUmRlcEntity::transmitSdu(const RlcSdu& sdu, size_t segmentSize) {
    // Assign SN to the SDU
    uint16_t sn = state.txNext;
    if (sdu.size() > segmentSize)
    	state.txNext = (state.txNext + 1) % (1 << 6);  // 6-bit SN (mod 64)

    // Segment the SDU based on the segment size and generate UMD PDUs
    std::vector<UmdPdu> umdPdus = generateUmdPdus(sdu, sn, segmentSize);

    // Submit UMD PDUs to the lower layer
    for (const auto& pdu : umdPdus) {
        submitToLowerLayer(pdu);
    }
}

// Function to generate UMD PDUs from an RLC SDU
std::vector<UmdPdu> TransmittingUmRlcEntity::generateUmdPdus(const RlcSdu& sdu, uint16_t sn, size_t segmentSize) {
    std::vector<UmdPdu> umdPdus;
    size_t sduSize = sdu.size();

    if (sduSize <= segmentSize)
    {
        // SDU is not segmented
        UmdPdu pdu;
        pdu.si = 0b00;  // SI = 00 (complete SDU)
        pdu.reserved = 0;
        pdu.data = sdu;  // Entire SDU as payload
        umdPdus.push_back(pdu);
    } else
    {
	    for (size_t offset = 0; offset < sduSize; offset += segmentSize) {
		UmdPdu pdu;
		pdu.sn = sn;
		pdu.si = (offset == 0) ? 0b01 : (offset + segmentSize >= sduSize) ? 0b10 : 0b11;  // SI field
		pdu.so = offset;  // Segment Offset
		pdu.data = std::vector<uint8_t>(sdu.begin() + offset, sdu.begin() + std::min(offset + segmentSize, sduSize));
		umdPdus.push_back(pdu);
	    }
  }

    return umdPdus;
}

// Function to submit UMD PDUs to the lower layer (MAC)
void TransmittingUmRlcEntity::submitToLowerLayer(const UmdPdu& pdu) {
    if (pdu.si != 0b00)
    	std::cout << "Transmitting PDU: SN=" << pdu.sn << ", SO=" << pdu.so << ", SI=" << static_cast<int>(pdu.si) << "\n";
}
