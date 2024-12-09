// ============================================================================
// ============================================================================
// parser.h: 
// This is the "module" (component) that is responsible to do all the parsing
// work on the intercepted (sniffed) packets.
// 
// ============================================================================
// ============================================================================
#pragma once

#include "../../utils/include/utils.h"

// ==========================
// Parser related definitions
// ==========================
#define PARSER_IPv4_ADDR_LEN 4
#define PARSER_IPv6_ADDR_LEN 16

// ====================================
// TCP/IP protocols related definitions
// ====================================
#define ETHERNET_HEADER_SIZE 14
#define ETHERNET_ETHER_TYPE_IPv4 0x0800
#define ETHERNET_ETHER_TYPE_IPv6 0x86DD
#define IPv4_HEADER_SIZE 20
#define UDP_HEADER_SIZE 8

// =======================
// DNS related definitions
// =======================
#define DNS_HEADER_SIZE sizeof(DnsHeader)
#define DNS_LABEL_MAX_LEN 64
#define DNS_PTR_NAME      0xc0
#define DNS_QNAME_MAX_LEN 256
#define DNS_MAX_NUM_QUESTIONS_PER_RESPONSE 1

#pragma pack(push, 1)
typedef struct DnsHeader
{
  uint16_t id;
  struct flags
  {
    uint16_t rcode  : 4;
    uint16_t z      : 3;
    uint16_t ra     : 1;
    uint16_t rd     : 1;
    uint16_t tc     : 1;
    uint16_t aa     : 1;
    uint16_t opcode : 4;
    uint16_t qr     : 1;
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
  // GuyA: This is a point of trade-off between space Vs. run-time, i.e. - from space perspective, it would
  // have been better to define it as a char* and allocate it in run-time according to the actual length.
  char resourceData [DNS_QNAME_MAX_LEN];
} DnsResourceRecord;
#pragma pack(pop)

typedef enum RecordType
{
    PARSER_RECORD_TYPE_IPv4,
    PARSER_RECORD_TYPE_IPv6,
    PARSER_RECORD_TYPE_CNAME,
    PARSER_RECORD_TYPE_UNSUPPORTED
} RecordType;

// =================
// Module public API
// =================
uint16_t extract16(IN const uint8_t* buffer, IN size_t offset);
uint32_t extract32(IN const uint8_t* buffer, IN size_t offset);
size_t parseDnsQuestion(IN const uint8_t* buffer, IN size_t offset, OUT DnsQuestion* dnsQuestion);
size_t parseDnsAnswer(IN const uint8_t* buffer, IN size_t offset, OUT DnsResourceRecord* dnsResourceRecord);
int parseDnsResponse(IN const uint8_t* packet);
