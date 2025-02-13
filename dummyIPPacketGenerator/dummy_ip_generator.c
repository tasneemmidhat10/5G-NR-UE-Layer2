#include <stdint.h>  /* for uint8_t, uint16_t, uint32_t types */

/**
 * struct ip_header - Structure for an IPv4 header
 * @version_and_ihl: 4 bits for IP version, 4 bits for IHL (Header Length)
 * @tos: Type of Service
 * @total_length: Total length of the IP packet (header + data)
 * @id: Identification field for fragmentation
 * @flags_and_offset: 3 bits for flags, 13 bits for fragment offset
 * @ttl: Time to Live (maximum number of hops)
 * @protocol: Protocol used in the payload (e.g., TCP = 6, UDP = 17)
 * @checksum: Header checksum for error checking
 * @src_ip: Source IP address
 * @dest_ip: Destination IP address
 */
struct ip_header
{
    uint8_t version_and_ihl;   /* Version (4 bits) and IHL (4 bits) */
    uint8_t tos;               /* Type of Service */
    uint16_t total_length;     /* Total length of the IP packet */
    uint16_t id;               /* Identification */
    uint16_t flags_and_offset; /* Flags (3 bits) and Fragment Offset (13 bits) */
    uint8_t ttl;               /* Time to Live */
    uint8_t protocol;          /* Protocol (e.g., TCP=6, UDP=17) */
    uint16_t checksum;         /* Header checksum */
    uint32_t src_ip;           /* Source IP address */
    uint32_t dest_ip;          /* Destination IP address */
};

/**
 * create_ip_packet - Creates an IP header for a packet
 * @header: Pointer to the ip_header structure to populate
 * @src_ip: Source IP address
 * @dest_ip: Destination IP address
 * @data_length: Length of the data payload
 * @protocol: Protocol used in the payload (e.g., TCP=6, UDP=17)
 *
 * Return: Nothing. The header structure is populated with the values.
 */
void create_ip_packet(struct ip_header *header, uint32_t src_ip,
                      uint32_t dest_ip, uint16_t data_length,
                      uint8_t protocol)
{
    header->version_and_ihl = (4 << 4) | 5; /* Version 4, IHL 5 (no options) */
    header->tos = 0;                       /* Default ToS */
    header->total_length = htons(sizeof(struct ip_header) + data_length);
    header->id = 0;                        /* Typically set to 0 or some random value */
    header->flags_and_offset = 0;          /* No fragmentation (0) */
    header->ttl = 64;                      /* Default TTL */
    header->protocol = protocol;           /* Set the protocol (e.g., TCP=6, UDP=17) */
    header->checksum = 0;                  /* Set to 0 initially (to be calculated later) */
    header->src_ip = src_ip;               /* Set source IP address */
    header->dest_ip = dest_ip;             /* Set destination IP address */
}

/**
 * main - Example usage of the IP header creation function
 *
 * Return: Always returns 0.
 */
int main(void)
{
    struct ip_header packet;
    uint32_t src_ip = 0xC0A80001;  /* 192.168.0.1 (example source IP) */
    uint32_t dest_ip = 0xC0A80002; /* 192.168.0.2 (example destination IP) */
    uint16_t data_length = 100;    /* Example payload data length (in bytes) */
    uint8_t protocol = 6;          /* Example protocol (6 for TCP) */

    create_ip_packet(&packet, src_ip, dest_ip, data_length, protocol);

    /* Normally, you would calculate the checksum here and send the packet */

    return (0);
}
