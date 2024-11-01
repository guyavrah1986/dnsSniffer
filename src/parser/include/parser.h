#pragma once

#include <pcap.h>

#include "../../utils/include/utils.h"

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
#define DNS_QNAME_MAX_LEN 256

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
  // GuyA: Aside from the additional extra space for the NULL terminator for actual
  // maximum string it is better to define it 256 so it will be divided by 32/64
  char question[DNS_QNAME_MAX_LEN];
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

size_t parseDnsQuestion(IN const uint8_t* buffer, IN size_t offset, OUT DnsQuestion* dnsQuestion);




void parse_response(uint16_t id, uint8_t *res);
int parseDnsResponse(const unsigned char *packet);
int parseDnsPacket(const unsigned char *packet);
void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet);
int sampleFuncUsingLibpcap();