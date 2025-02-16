#include "PDCP.hpp"
#include <iostream>
#include <bitset>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/cmac.h>
#include <iomanip>
#include <cstring>
#include <vector>
#include <stdlib.h>
// I should include here the cryptographic algorithms
unsigned int seed;
size_t i;
rohc_status_t status;
static int gen_random_num(const struct rohc_comp *const comp,void *const user_context){
    return rand();
};
struct rohc_ts arrival_time = {.sec=0, .nsec=0};
/*PdcpEntity class deconstructor*/
//PdcpEntity::~PdcpEntity(){};

/*Reads from the PDCP message queue with the upper layer and places the Ip Packet(array of bytes) in the TX_BUFFER*/
void PdcpUplink::receive_PDU(const std::vector<uint8_t >& sdu) {
    TX_buffer.push(sdu);
};

std::vector<uint8_t> PdcpUplink::compress_header(const rohc_ts arrival_time)
{
    std::vector<uint8_t> ipPacket;
    unsigned char ip_buffer_comp[4096];
    unsigned char rohc_buffer[BUFFER_SIZE];
    if (TX_buffer.empty()) {
        std::fprintf(stderr, "TX_buffer is empty\n");
        return {}; // Return an empty vector
    }
    if (ipPacket.size() > 4096) {
        std::fprintf(stderr, "IP packet size exceeds buffer size\n");
        return {}; // Return an empty vector
    }
    ipPacket =TX_buffer.front();
    TX_buffer.pop();
    memcpy(ip_buffer_comp, ipPacket.data(), ipPacket.size());
    struct rohc_buf ip_packet = rohc_buf_init_full(ip_buffer_comp, ipPacket.size(), arrival_time);
    struct rohc_buf rohc_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE);
    status = rohc_compress4(compressor, ip_packet, &rohc_packet);
    if(status == ROHC_STATUS_OK)
    {
        std::cout<<"\nROHC Success\n";
    }
    else 
    {
        std::printf("compression Failed \n");
        std::cout<<status;
        return {};   
    }
     //Temporary array to hold the compressed buffer
     if(!compressed_packet.empty())
     {
        compressed_packet.clear();
     }
     compressed_packet.assign(rohc_packet.data, rohc_packet.data + rohc_packet.len);
    memset(ip_buffer_comp, 0, sizeof(ip_buffer_comp));
    rohc_buf_reset(&rohc_packet);
    rohc_buf_reset(&ip_packet);
    return compressed_packet;
};
/*This function process the resultant compressed SDU; it adds the specifc PDCP header and managing the state variables*/
std::vector<uint8_t> PdcpUplink::processPDU(const std::vector<uint8_t> &compressed_IpPacket)
{
    std::vector<uint8_t> pdu;
    if(!compressed_IpPacket.empty()){
        uint16_t header = 0;
        std::printf("\n   The SN %d ", SN);
        std::printf("\n %d ", static_cast<uint16_t>((SN & 0x00000FFF)));
        header = header | static_cast<uint16_t>((SN & 0x00000FFF));
        header = header | (0 << 12);
        header = header | (0 << 13); /** The three bits are for R; reserved for now */
        header = header | (0 << 14);
        header = header | (0 << 15); //The last bit is indicating that the PDU is a DATA PDU
        //serializing the pdu
        pdu.push_back(static_cast<uint8_t>((header >> 8) & 0x00FF)); //accessing the High-byte (Big-endian)
        pdu.push_back(static_cast<uint8_t>(header & 0x00FF)); //accessing the Lower-byte (Big-endian)
        pdu.insert(pdu.end(), compressed_IpPacket.begin(), compressed_IpPacket.end()); // appending the payload; the compressed ip packet
        std::printf("\n Serializing the PDU...\n");
        for (i = 0; i < pdu.size(); i++){
            std::printf(" 0x%02x", pdu[i]);
            if(i != 0 && ((i+1)%8 == 0)){
                std::printf("\n");
            }
        }
    }
    else {
        return{};
    }
    TX_NEXT++; // Increment the TX_NEXT value to point to the next SN to be transmitted
    std::printf(" TX_NEXT VALUE %d\n", TX_NEXT);
    SN++;
    std::printf(" NEXT SN VALUE %d\n", SN); //For ordering and sequencing
    return pdu;
};
/*Initialize the ROHC compressor with the user context and returns a pointer to the compressor*/
rohc_comp *PdcpUplink::initalizeROHC(const rohc_cid_type_t CID, rohc_cid_t MAX_CID)
{
    compressor = rohc_comp_new2(CID, MAX_CID, gen_random_num, NULL);
    if(compressor == NULL)
        {
            std::fprintf(stderr, "failed to create the ROHC compressor\n");
        }
    if(!rohc_comp_enable_profile(compressor, ROHC_PROFILE_IP))
        {
            std::printf("\n Failed to enable IP-only profile\n");
        };
    if(!rohc_comp_enable_profile(compressor, ROHC_PROFILE_UNCOMPRESSED)){
            std::fprintf(stderr, "failed to enable the UNCOMPRESSED profile\n");
        }
    if(!rohc_comp_enable_profiles(compressor, ROHC_PROFILE_UDP, ROHC_PROFILE_ESP, -1)){
            std::fprintf(stderr, "failed to enable the IP/UDP and IP/ESP profiles\n");
        }
    return compressor;
};
/*Free the allocated memory for the ROHC COMPRESSOR*/
void PdcpUplink::destroyCompressor(rohc_comp *comp) {
    if(comp == NULL)
    {
        std::fprintf(stderr, "\n There isn't a compressor initialized to destroy\n");
    }
    rohc_comp_free(comp);
}
std::vector<uint8_t> PdcpUplink::pdcpCipher(const std::vector<uint8_t>& input, int length, unsigned char* key)
{
    // Print the symmetric key
    std::printf("\nThe symmetric key:\n");
    for (int i = 0; i < 32; i++) {  // AES-256 key size
        std::printf("0x%02x ", key[i]);
        if (i != 0 && ((i + 1) % 8 == 0)) {
            std::printf("\n");
        }
    }

    // Copy payload
    unsigned char payLoad[length];
    memcpy(payLoad, input.data(), input.size());

    std::printf("\nThe payload after copying: ");
    for (int i = 0; i < length; i++) {
        std::printf("0x%02x ", payLoad[i]);
    }

    // Initialize IV
    unsigned char iv[16] = {0};  // Ensure the full IV is zeroed
    memcpy(iv, &TX_NEXT, 4);     // Copy only 4 bytes from TX_NEXT
    std::printf("\nIV:\n");
    for (int i = 0; i < 16; i++) {
        std::printf("0x%02x ", iv[i]);
    }
    std::printf("\n");

    std::printf("\nThe value of the count used in encryption is %d\n", TX_NEXT);

    // Create the output buffer with extra space for padding
    unsigned char outputBuffer[length + 16] = {0};  

    // Initialize encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx || !EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        std::cerr << "Encryption init failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    int out_len;
    EVP_EncryptUpdate(ctx, outputBuffer, &out_len, payLoad, length);
    //EVP_EncryptFinal_ex(ctx, payLoad + out_len, &out_len);
    EVP_CIPHER_CTX_free(ctx);
    std::printf("\n");
    std::vector<uint8_t> cipheredMessage;
    std::copy(outputBuffer, outputBuffer + sizeof(outputBuffer), std::back_inserter(cipheredMessage));
    return cipheredMessage;
}
std::vector<uint8_t> PdcpEntity:: generateAESCmac(std::vector<uint8_t> &data, const uint8_t *key)
{
    uint8_t mac[16];
    CMAC_CTX* ctx = CMAC_CTX_new();
    CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);
    CMAC_Update(ctx, data.data(), data.size());
    size_t mac_len;
    CMAC_Final(ctx, mac, &mac_len);
    CMAC_CTX_free(ctx);
    //data.insert(data.end(), mac, mac + 16);
    return std::vector<uint8_t>(mac, mac + 16);
};
/* Setting the keys of the integrity protection and ciphering algorithm*/
void PdcpEntity::setSecurityContext() {
    //securityContext.integrityKey = integrityKey; //Set a random key that will be used by AES-256 algorithm to encrypt and decrypt the message
    RAND_bytes(securityContext.key, sizeof(securityContext.key));
}
/*Initializing the rohc decompresser with the right context ID and specifiying the mode */
rohc_decomp *PdcpDownlink::initializeRohcDecomp(const rohc_cid_type_t CID, const rohc_cid_t max_cid, const rohc_mode_t mode)
{
    decompressor = rohc_decomp_new2(CID, max_cid, mode);
    if (decompressor == NULL) 
	{
		std::printf("failed create the ROHC decompressor\n");
	}
	if (!rohc_decomp_enable_profile(decompressor, ROHC_PROFILE_UNCOMPRESSED))
	{
		std::printf("failed to enable the Uncompressed profile\n");
	}
	if (!rohc_decomp_enable_profile(decompressor, ROHC_PROFILE_IP))
	{
		std::printf("failed to enable the IP-only profile\n");

	}
	if (!rohc_decomp_enable_profiles(decompressor, ROHC_PROFILE_UDP, ROHC_PROFILE_ESP, -1))
	{
		std::printf("failed to enable the IP/UDP and IP/ESP profiles\n");

	}
    return decompressor;
}
/*Frees the allocated memory for the rohc decompressor*/
void PdcpDownlink::destroyDecompressor(rohc_decomp *decomp) {
    if (decomp == NULL)
    {
        std::printf("There isn't a decompressor initalized to destroy");
    }
    rohc_decomp_free(decomp);
}
void PdcpDownlink::setRX_DELIEV() {
    RX_DELIEV.HFN = 0;
    RX_DELIEV.SN = 0;
};
/*Function to decompress the incoming ip packet header*/
std::vector<uint8_t> PdcpDownlink::decompressHeader(rohc_ts arrival_time)
{
/* The buffer that will contain the rohc packet to be decompressed*/
    std::vector<uint8_t> ipPacketdecomp;
    struct rohc_buf* rcvd_feedback = NULL;
    struct rohc_buf* feedback_send = NULL;
    unsigned char ip_buffer_decomp[BUFFER_SIZE];
    unsigned char rohc_buffer[BUFFER_SIZE];
    std::cout << "\nRX_BUFFER size before pop: " << RX_BUFFER.size() << std::endl;
    ipPacketdecomp = RX_BUFFER.front();
    std::printf("\n %d ipPacket size ", ipPacketdecomp.size());
    RX_BUFFER.pop();
    std::cout << "\nRX_BUFFER size after pop: " << RX_BUFFER.size() << std::endl;
    memcpy(ip_buffer_decomp, ipPacketdecomp.data(),ipPacketdecomp.size());
    struct rohc_buf ip_packet = rohc_buf_init_empty(rohc_buffer, BUFFER_SIZE);
    struct rohc_buf rohc_packet = rohc_buf_init_full(ip_buffer_decomp, ipPacketdecomp.size(), arrival_time);
    status = rohc_decompress3(decompressor, rohc_packet, &ip_packet, rcvd_feedback, feedback_send);
	if (status == ROHC_STATUS_OK)
	{
		if (!rohc_buf_is_empty(ip_packet)) {
			std::printf("\nDecompression succeded");
		}
	}
	else
	{
		std::printf("Decompression failed\n");
		std::cout << status;
	}
    if (!decompressed_packet.empty()){
        decompressed_packet.clear();
    }
    decompressed_packet.assign(rohc_packet.data, rohc_packet.data + rohc_packet.len);
    memset(ip_buffer_decomp, 0, sizeof(ip_buffer_decomp));
    rohc_buf_reset(&rohc_packet);
    rohc_buf_reset(&ip_packet);
    decompressed_packet.erase(decompressed_packet.begin(), decompressed_packet.begin() + 3);
	return decompressed_packet;
};
/*Extracts the PDCP header from the incoming PDU packet and calculates RCVD_SN, RCVD_HFN and RCVD_COUNT; the function returns the modifed SDU*/
std::vector<uint8_t> PdcpDownlink::removePDCPHeader(std::vector<uint8_t>& SDU)
{
    uint16_t header = 0;
    header = (SDU[0] << 8) | SDU[1]; 
    std::printf("\n The first and last elements = %d , %d", SDU[0],SDU[1]);
    std::printf("\n PDCP Header = %d", header);
    RCVD_SN = (header & 0x0FFF);
    std::printf("\n RCVD_SN = %d", RCVD_SN);
    SDU.erase(SDU.begin(), SDU.begin() + 2);
    RCVD_COUNT = calculateCountRCV(RCVD_SN);
    std::printf("\n RCVD_COUNT = %d", RCVD_COUNT); // calculates the recieved count value of the PDU and updates the HFN number
    uint32_t packedRX_DELIEV = (RX_DELIEV.HFN<<12) | (RX_DELIEV.SN); 
    std::bitset<32> binaryRX_Deliev(RX_DELIEV.SN);
    std::cout<<"\n RX_DELIEV value in binary format "<<binaryRX_Deliev;
    if ( RX_DELIEV.HFN > lastTrackedHFN){ //To handle memory effiecentyl when the uordered set growths beyond the currrent HFN and to handle the SN wrapround
        recivedCounts.clear();
        lastTrackedHFN = RX_DELIEV.HFN;
    }
    if (RCVD_COUNT < packedRX_DELIEV){
        return {};
    };
    if (recivedCounts.find(RCVD_COUNT)!= recivedCounts.end()) //To check if the received pdu is a duplicate
    {
        return {};
    }
    recivedCounts.insert(RCVD_COUNT);
    return SDU;
}
std::vector<uint8_t> PdcpDownlink::pdcpCipherDownlink(const std::vector<uint8_t>& input, int length, unsigned char* key)
{
    // Print the symmetric key
    std::printf("\nThe symmetric key:\n");
    for (int i = 0; i < 32; i++) {  // AES-256 key size
        std::printf("0x%02x ", key[i]);
        if (i != 0 && ((i + 1) % 8 == 0)) {
            std::printf("\n");
        }
    }

    // Prepare IV
    unsigned char iv[16] = {0};  // Ensure the full IV is zeroed
    memcpy(iv, &RX_NEXT, 4);     // Copy only 4 bytes from RX_NEXT
    std::printf("\nThe IV VECTOR: %d %d\n", iv[0], iv[15]);
    std::printf("\nIV:\n");
    for (int i = 0; i < 16; i++) {
        std::printf("0x%02x ", iv[i]);
    }
    std::printf("\n");

    // Allocate buffers
    unsigned char outputBuffer[length + 16] = {};  // Extra space for padding
    unsigned char payLoad[length];
    memcpy(payLoad, input.data(), input.size());

    // Initialize decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx || !EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        std::cerr << "Decryption init failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    // Perform decryption
    int out_len = 0;
    EVP_DecryptUpdate(ctx, outputBuffer, &out_len, payLoad, length);

    int final_out_len = 0;
    // EVP_DecryptFinal_ex(ctx, outputBuffer + out_len, &final_out_len);
    // out_len += final_out_len;

    // Cleanup
    EVP_CIPHER_CTX_free(ctx);

    // Copy decrypted data into vector
    std::vector<uint8_t> decipheredMessage(outputBuffer, outputBuffer + out_len);
    return decipheredMessage;
}

std::vector<uint8_t> PdcpDownlink::verifyAESCmac(const std::vector<uint8_t>& data, const uint8_t* key, const uint8_t* receivedMac) {
    // If receivedMac is not provided, assume it is the last 16 bytes of the data
    const uint8_t* macToVerify = receivedMac;
    std::vector<uint8_t> extractedMac;

    if (receivedMac == nullptr) {
        if (data.size() < 16) {
            throw std::runtime_error("Data is too small to contain a MAC");
        }
        // Extract the last 16 bytes as the MAC
        extractedMac.assign(data.end() - 16, data.end());
        macToVerify = extractedMac.data();
    }

    // Generate the MAC for the data (excluding the last 16 bytes if they are the MAC)
    std::vector<uint8_t> dataWithoutMac(data.begin(), data.end() - (receivedMac ? 0 : 16));
    std::vector<uint8_t> generatedMac = PdcpEntity::generateAESCmac(dataWithoutMac, key);

    // Compare generated MAC with the received or extracted MAC
    if(memcmp(generatedMac.data(), macToVerify, 16) == 0){
        std::printf("\n The intergity of the packet was verified correctly");
        return dataWithoutMac;
    }
    else{
        return data;
    };
}

std::vector<uint8_t> PdcpDownlink::processSDU(const std::vector<uint8_t> &SDU)
{
    uint32_t packedRX_DELIEV = (RX_DELIEV.HFN<<12) | (RX_DELIEV.SN);
    if (RCVD_COUNT >= RX_NEXT){
        RX_NEXT = RCVD_COUNT + 1;
        if (config.OutOfOrderDelivery){
            return SDU; // Send to upper layers or to be decompressed without reordering 
        }
        else {
            reorderingMap[RCVD_COUNT] = SDU;
        }
    }
    if (RCVD_COUNT == packedRX_DELIEV){
        while (reorderingMap.find(packedRX_DELIEV)!= reorderingMap.end())
        {
            auto SDU = reorderingMap[packedRX_DELIEV];
            reorderingMap.erase(packedRX_DELIEV);
            //send to a message queue
        }
        packedRX_DELIEV = RCVD_COUNT + 1;
        RX_DELIEV.HFN = (packedRX_DELIEV & 0xFFFFF000);
        RX_DELIEV.SN = (packedRX_DELIEV & 0x00000FFF);
        std::printf("\n The NEXT RX DELIEV.SN VALUE IS %d ", RX_DELIEV.SN);
    }
    std::printf("\n The NEXT RX_NEXT VALUE IS %d ", RX_NEXT);
    return SDU;
}
/*Add the revived PDU to the reciption buffer*/
void PdcpDownlink::addToRX_BUFFER(const std::vector<uint8_t> &SDU)
{
    RX_BUFFER.push(SDU);
};
