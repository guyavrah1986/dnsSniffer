#pragma once

#include <arpa/inet.h>

extern "C"
{
    #include "../parser/include/parser.h"
}

TEST(ParserTests, sanityParseSingleDnsResponsePayload)
{
    uint8_t dnsPacket [] =
    {
        // DNS payload:

        // Header (12 bytes)
        0x23, 0x1f, // Transaction ID
        0x81, 0x80, // Flags
        0x00, 0x01, // Questions count
        0x00, 0x01, // Answers count
        0x00, 0x00, // Authority records count (NS)
        0x00, 0x01, // Additional records count
        // Query (16 bytes)
        0x06, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f,
        0x6d, 0x00, 0x00, 0x01, 0x00, 0x01, 
        // Answer (16 bytes)
        0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1d,
        0x00, 0x04, 0x8e, 0xfa, 0x4b, 0x2e,
        // Additional records
        0x00, 0x00, 0x29, 0x0f, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    uint16_t expectedTransactionIdValDecimal = 8991;
    uint16_t expectedNumOfQUestionsValDecimal = 1;
    uint16_t expectedNumOfQAnswersValDecimal = 1;

    // Extract the DNS packet's header section
    DnsHeader* dnsHeader = (DnsHeader*)dnsPacket;
    EXPECT_EQ(expectedTransactionIdValDecimal, ntohs(dnsHeader->id));
    EXPECT_EQ(expectedNumOfQUestionsValDecimal, ntohs(dnsHeader->qdcount));
    EXPECT_EQ(expectedNumOfQAnswersValDecimal, ntohs(dnsHeader->ancount));

    // Extract the DNS question
    size_t offsetFromStartOfDnsPayload = DNS_HEADER_SIZE;
    size_t questionLength = 16; 
    const char expectedQuestionDomainName [] = "google.com";
    DnsQuestion dnsQuestion;
    memset(&dnsQuestion, 0, sizeof(DnsQuestion));
    size_t ret = parseDnsQuestion(dnsPacket, offsetFromStartOfDnsPayload, &dnsQuestion);
    EXPECT_EQ(offsetFromStartOfDnsPayload + questionLength, ret);
    EXPECT_STREQ(expectedQuestionDomainName, dnsQuestion.question);

    // Extract the DNS answer
    uint16_t expectedType = 0x01;
    uint16_t expectedRecordClass = 0x01;
    uint32_t expectedTtl = 29;
    uint16_t expectedRdlength = 4;
    const char expectedResourceData [] = "142.250.75.46";
    size_t answerLength = 16;
    size_t offset = ret;
    DnsResourceRecord dnsResourceRecord;
    memset(&dnsResourceRecord, 0, sizeof(dnsResourceRecord));
    ret = parseDnsAnswer(dnsPacket, offset, &dnsResourceRecord);
    EXPECT_EQ(expectedType, dnsResourceRecord.type);
    EXPECT_EQ(expectedRecordClass, dnsResourceRecord.recordClass);
    EXPECT_EQ(expectedTtl, dnsResourceRecord.ttl);
    EXPECT_EQ(expectedRdlength, dnsResourceRecord.rdlength);
    EXPECT_EQ(offset + answerLength, ret);
    printf("the resource data extracted from the answer is:%s\n", dnsResourceRecord.resourceData);
    EXPECT_STREQ(expectedResourceData, dnsResourceRecord.resourceData);
}

/*
TEST(ParserTests, parseSingleDnsResponsePacketThatRidesOnUdpAndContainsSingleRecordOfTypeA)
{ 
    const unsigned char packet[] =
    {
        // Ethernet header (14 bytes)
        0xfa, 0x16, 0x3e, 0x47, 0xdb, 0x61, //Destination MAC address
        0x38, 0x10, 0xf0, 0x8c, 0x93, 0x40, // Source MAC address
        0x08, 0x00,                         // type (IPv4)
        
        // IPv4 header (20 bytes):
        0x45,                   // version + header length
        0x00,                   // DSF
        0x00, 0x53,             // total length
        0x36, 0x25,             // Identification
        0x00,                   // flags
        0x00,                   // fragment offset
        0x7d,                   // TTL
        0x11,                   // Protocol
        0x33, 0x12,             // Header checksum
        0x93, 0xea, 0xf4, 0x04, // IP Source address
        0x0a, 0x4c, 0x42, 0x28, // IP Destination address
        
        // UDP header:
        0x00, 0x35, // Source port
        0xce, 0x8c, // Destination port
        0x00, 0x3f, // Length
        0x41, 0x88, // Checksum

        // DNS:
        0x23, 0x1f, // Transaction ID
        0x81, 0x80, // Flags
        0x00, 0x01, // Questions count
        0x00, 0x01, // Answers count
        0x00, 0x00, // Authority records count (NS)
        0x00, 0x01, // Additional records count
        // Query
        0x06, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f,
        0x6d, 0x00, 0x00, 0x01, 0x00, 0x01, 
        // Answer
        0xc0, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1d,
        0x00, 0x04, 0x8e, 0xfa, 0x4b, 0x2e,
        // Additional records
        0x00, 0x00, 0x29, 0x0f, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    int ret = parseDnsResponse(packet);
    EXPECT_EQ(0, ret);
}
*/