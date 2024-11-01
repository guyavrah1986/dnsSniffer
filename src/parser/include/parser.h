#pragma once

#include <pcap.h>

// ====================================
// TCP/IP protocols related definitions
// ====================================
#define ETHERNET_HEADER_SIZE 14
#define IPv4_HEADER_SIZE 20
#define UDP_HEADER_SIZE 8

// =======================
// DNS related definitions
// =======================
#define DNS_HEADER_SIZE sizeof(DnsHeader)
#define DNS_LABEL_MAX_LEN 64
#define DNS_PTR_NAME      0xc0
#define DNS_QNAME_MAX_LEN 255

#pragma pack(push, 1)
typedef struct DnsHeader
{
  uint16_t id;
  struct flags
  { // GuyA: assuming there will not be PDP machine in production
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t rcode  : 4;
    uint16_t z      : 3;
    uint16_t ra     : 1;
    uint16_t rd     : 1;
    uint16_t tc     : 1;
    uint16_t aa     : 1;
    uint16_t opcode : 4;
    uint16_t qr     : 1;
#else
    uint16_t qr     : 1;
    uint16_t opcode : 4;
    uint16_t aa     : 1;
    uint16_t tc     : 1;
    uint16_t rd     : 1;
    uint16_t ra     : 1;
    uint16_t z      : 3;
    uint16_t rcode  : 4;
#endif  /* __LITTLE_ENDIAN */
  } flags;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} DnsHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct DnsQuestion
{
  uint16_t questionType;
  uint16_t questionClass;
} DnsQuestion;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct DnsResourceRecord
{
  uint16_t  name;
  uint16_t  type;
  uint16_t  recordClass;
  int32_t   ttl;
  uint16_t  rdlength;
} DnsResourceRecord;
#pragma pack(pop)

// ===================
// Utilities functions
// ===================
uint16_t extract16(const uint8_t* buffer, size_t offset);
uint32_t extract32(const uint8_t* buffer, size_t offset);


void parse_response(uint16_t id, uint8_t *res);
int parseDnsResponse(const unsigned char *packet);
int parseDnsPacket(const unsigned char *packet);
void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet);
int sampleFuncUsingLibpcap();