#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <winsock2.h>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <rohc/rohc.h>
#include <rohc/rohc_comp.h>
#include <rohc/rohc_decomp.h>
#include "rohc_init_params.hpp"

#define SN_SIZE 12

class PdcpEntity {
protected: 
    struct PDCP_PDU {
        struct PDCP_Header { // using bitfields
            uint16_t SN : 12; // The sequence number of a PDCP PDU
            uint16_t R : 3;   // The reserved 3 bits in the pdcp header
            uint16_t d_c : 1; // The 1 bit for indicating whether this is from the data or control plane
        } header;
        std::vector<uint8_t> payload;
    };
    struct SecurityContext {
        std::vector<uint8_t> integrityKey;
        unsigned char key[32]; // AES-256 Key
    } securityContext;
public:
    struct PdcpConfig {
        bool OutOfOrderDelivery; // When activated Data PDUs are delivered to the upper layers as they arrive withput reordering
        uint32_t discard_timer;
        uint32_t t_reordering;
        bool integrity_protection;
        bool ciphering;
        rohc_profile_t rohc_profile;
    };
     PdcpConfig config;
    /*The parent constructor*/
    PdcpEntity(const PdcpConfig& config) : config(config){};
    //virtual ~PdcpEntity();
    void setSecurityContext();
    
};

class PdcpUplink : public PdcpEntity {
private:
      /* The transmission PDCP entity state variables*/
    const uint32_t SN_MAX = 4096; // Maximum value of SN (12 bits)
    uint32_t SN = 0;              // Initialize SN
    uint32_t TX_NEXT = 0;         // Initialize TX_NEXT
    uint32_t COUNT;               // COUNT is calculated from TX_NEXT and SN
    std::queue<std::vector<uint8_t>> TX_buffer; // The transmission buffer
    uint32_t calculateCOUNT() const {
        uint32_t HFN = TX_NEXT / SN_MAX; // Calculate HFN
        return (HFN * SN_MAX) + SN;      // Calculate COUNT
    }
    std::vector<uint8_t> compressed_packet;
    struct rohc_comp *compressor;
public:
    PdcpUplink(const PdcpConfig& config): PdcpEntity(config){};
    void receive_PDU(const std::vector<uint8_t>& sdu); // Recieving the uncompressed IP Packet from the Upper layers and places it in the TX_Buffer
    std::vector<uint8_t> compress_header(const rohc_ts arrival_time); // I should pull the PDU from the TX_BUFFER and serialize it to put it into the ip_buffer
    std::vector<uint8_t> processPDU(const std::vector<uint8_t>& compressed_IpPacket);
    struct rohc_comp* initalizeROHC(const rohc_cid_type_t CID, rohc_cid_t MAX_CID);
    void destroyCompressor(struct rohc_comp* comp);
    std::vector<uint8_t> pdcpCipher(const std::vector<uint8_t>&input, int length, unsigned char* key);
};

class PdcpDownlink : public PdcpEntity {
private:
    /*The recieving PDCP entity state variables*/
    std::queue<std::vector<uint8_t>> RX_BUFFER;
    uint32_t RX_NEXT = 0; // This state variable indicates the COUNT value of the next PDCP SDU expected.
    struct{
        uint32_t  HFN : 20;
        uint32_t  SN : 12;
    } RX_DELIEV; // This state variable indicates the COUNT value of the first PDCP SDU not delivered to the upper layers, but still waited for.
    uint32_t RX_REORD; // This state variable indicates the COUNT value following the COUNT value associated with the PDCP Data PDU which triggered t-Reordering.
    uint32_t RCVD_COUNT;
    uint16_t RCVD_SN;
    uint16_t RCVD_HFN;
    uint32_t window_size = 2048;
    uint32_t calculateCountRCV(const uint16_t& RCVD_SN) 
    {
        //uint16_t SN_lower_bound = (RX_DELIEV.SN + (4096 - window_size)) % 4096;
        //uint16_t SN_upper_bound = (RX_DELIEV.SN + window_size) % 4096;
        if (RCVD_SN < (RX_DELIEV.SN - window_size))
        {
            RCVD_HFN = RX_DELIEV.HFN + 1;
        }
        else if (RCVD_SN >= RX_DELIEV.SN + window_size)
        {
            RCVD_HFN = RX_DELIEV.HFN - 1;
        }
        else
        {
            RCVD_HFN = RX_DELIEV.HFN;
        }
        RCVD_COUNT =(RCVD_HFN >> SN_SIZE) | RCVD_SN;
        return RCVD_COUNT;
    };
    struct rohc_decomp* decompressor;
    std::vector<uint8_t> decompressed_packet;
    std::unordered_set<uint32_t> recivedCounts;
    uint32_t lastTrackedHFN = 0;
    std::unordered_map<uint32_t, std::vector<uint8_t>> reorderingMap;
public:
    PdcpDownlink(const PdcpConfig& config) : PdcpEntity(config){};
    struct rohc_decomp* initializeRohcDecomp(const rohc_cid_type_t CID, const rohc_cid_t max_cid, const rohc_mode_t mode);
    void destroyDecompressor(struct rohc_decomp* decomp);
    void setRX_DELIEV();
    void sendToPDCP(const struct RLC& rlc); // This can be done using POSIX message queues 
    std::vector<uint8_t> decompressHeader(const rohc_ts arrival_time);
    void readFromMessageQueue(); // Will read from the PDCP_reciveing message queue and will place the entity in the deciphering buffer
    std::vector<uint8_t> removePDCPHeader(std::vector<uint8_t>& SDU); //The array of bytes the first 2 bytes are the pdcp header, I will extract them and place it in the header struct to extract the SN and count the RCVD_COUNT
    std::vector<uint8_t> pdcpCipherDownlink(const std::vector<uint8_t>&input,int length, unsigned char* key);
    std::vector<uint8_t> processSDU(const std::vector<uint8_t>& SDU); //The function handle duplication, reordering and processing of the SDU unit
    void addToRX_BUFFER(const std::vector<uint8_t>& SDU);
};