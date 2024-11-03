#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arpa/inet.h>

#include "include/parser.h"

static void ipv6ToStr(IN const uint8_t* buffer, IN size_t startOffset, OUT char resourceData [])
{
    sprintf(resourceData,
            "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
            buffer[startOffset], buffer[startOffset + 1],
            buffer[startOffset + 2], buffer[startOffset + 3],
            buffer[startOffset + 4], buffer[startOffset + 5],
            buffer[startOffset + 6], buffer[startOffset + 7],
            buffer[startOffset + 8], buffer[startOffset + 9],
            buffer[startOffset + 10], buffer[startOffset + 11],
            buffer[startOffset + 12], buffer[startOffset + 13],
            buffer[startOffset + 14], buffer[startOffset + 15]);
}

static RecordType getRecordType(IN const uint16_t recordTypeVal)
{
    RecordType retVal = PARSER_RECORD_TYPE_UNSUPPORTED;
    switch (recordTypeVal)
    {
        case 1: retVal = PARSER_RECORD_TYPE_IPv4;
                 break;
        case 28: retVal = PARSER_RECORD_TYPE_IPv6;
                 break;
        case 5: retVal = PARSER_RECORD_TYPE_CNAME;
                 break;
        default: retVal = PARSER_RECORD_TYPE_UNSUPPORTED;
                 break;
    }

    return retVal;
}

static size_t calculateOffsetToDnsPayload()
{
    const char funcNmae [] = "parser::calculateOffsetToDnsPayload - ";
    size_t offsetToDnsPayload = 0;

    // GuyA:According to some research done online, it appears that when a WiFi 
    // packet (802.11x) is being handed from the driver to the rest of the
    // network stack, it is being "considered" as a "fake" Ethernet (802.3) 
    // frame. This make sense, becuase WiFi was developed to be the "wireless
    // Ethernet" --> so in any case, from Link layer header length perspective 
    // assume it is an Ethernet frame --> i.e, length is 14 bytes
    offsetToDnsPayload += ETHERNET_HEADER_SIZE;

    // GuyA: In the case of DNS packets that:
    // 1. Encapsulated within IPv6
    // 2. Encapsulated within IPv4 and the options section is present
    // --> the offset needs to be adjusted!
    offsetToDnsPayload += IPv4_HEADER_SIZE;

    // GuyA: The assumption (which is also applied in the libpcap filter) is
    // that only DNS packets on top of UDP are desired.
    offsetToDnsPayload += UDP_HEADER_SIZE;

    printf("%s returning offset:%lu\n", funcNmae, offsetToDnsPayload);
    return offsetToDnsPayload;
}

static size_t extractRecordAddress(IN const uint8_t* buffer, const RecordType recordType, IN size_t startOffset, OUT char resourceData [])
{
    const char funcName [] = "extractRecordAddress - ";
    size_t bytesParsed = 0;
    switch (recordType)
    {
        case PARSER_RECORD_TYPE_IPv4: printf("%s IPv4 address\n", funcName);
                                      sprintf(resourceData, "%u.%u.%u.%u", buffer[startOffset], buffer[startOffset + 1], buffer[startOffset + 2], buffer[startOffset + 3]);
                                      bytesParsed = PARSER_IPv4_ADDR_LEN;
                                      break;
        case PARSER_RECORD_TYPE_IPv6: printf("%s IPv6 address\n", funcName);
                                      ipv6ToStr(buffer, startOffset, resourceData);
                                      bytesParsed = PARSER_IPv6_ADDR_LEN;
                                      break;
        case PARSER_RECORD_TYPE_CNAME: printf("%s CNAME address\n", funcName);
                                       bytesParsed = PARSER_IPv6_ADDR_LEN;
                                       break;
        default: printf("%s unsupported record type\n", funcName);
                 break;
    }

    return bytesParsed;
}

// Helper function to extract a 16-bit value from a buffer
uint16_t extract16(IN const uint8_t* buffer, IN size_t offset)
{
    return (buffer[offset] << 8) | buffer[offset + 1];
}

// Helper function to extract a 32-bit value from a buffer
uint32_t extract32(IN const uint8_t* buffer, IN size_t offset)
{
    return (buffer[offset] << 24) | (buffer[offset + 1] << 16) |
           (buffer[offset + 2] << 8) | buffer[offset + 3];
}

// Function to parse QName (domain name)
size_t parseQName(IN const uint8_t* buffer, IN size_t offset, OUT char* qname)
{
    size_t pos = offset;
    size_t nameIndex = 0;
    while (buffer[pos] != 0)
    {
        int labelLen = buffer[pos++];
        for (int i = 0; i < labelLen; ++i)
        {
            qname[nameIndex++] = buffer[pos++];
        }
        
        qname[nameIndex++] = '.'; // Separate labels with '.'
    }

    qname[nameIndex - 1] = '\0'; // Null-terminate the string and remove last '.'
    return pos + 1; // Position after QName (0 terminator)
}

// Function to parse DNS question section
size_t parseDnsQuestion(IN const uint8_t* buffer, IN size_t offset, OUT DnsQuestion* dnsQuestion)
{
    const char funcName [] = "parseDnsQuestion - ";
    offset = parseQName(buffer, offset, dnsQuestion->question);
    uint16_t qtype = extract16(buffer, offset);
    uint16_t qclass = extract16(buffer, offset + 2);

    printf("%s question Name: %s\n", funcName, dnsQuestion->question);
    printf("%s question Type: %u\n", funcName, qtype);
    printf("%s question Class: %u\n", funcName, qclass);

    return offset + 4; // Move past QType and QClass
}

// Function to parse DNS answer section
size_t parseDnsAnswer(IN const uint8_t* buffer, IN size_t offset, OUT DnsResourceRecord* dnsResourceRecord)
{
    // NOTE: The assumption here is that the name field in the answer will be
    // of type DNS_PTR_NAME (0xc0) --> so it means that the next byte is the 
    // offset from the begining of the DNS packet, and then there are the 
    // rest of the fields of the DNS answer (record)
    const char funcName [] = "parseDnsAnswer - ";
    if (DNS_PTR_NAME != buffer[offset])
    {
        printf("%s currently only answers with type DNS_PTR_NAME are supported, and this is not the case - aborting parse\n", funcName);
        return 0;
    }
    
    // Move past the name field
    offset += 2;
    printf("%s offset is now:%lu\n", funcName, offset);
    dnsResourceRecord->type = extract16(buffer, offset);
    dnsResourceRecord->recordClass = extract16(buffer, offset + 2);
    dnsResourceRecord->ttl = extract32(buffer, offset + 4);
    dnsResourceRecord->rdlength = extract16(buffer, offset + 8);

    // Move past Type, Class, TTL, and RDLength
    offset += 10;

    // GuyA: For debug - needs to be removed
    printf("%s answer Type: %u\n", funcName, dnsResourceRecord->type);
    printf("%s answer Class: %u\n", funcName, dnsResourceRecord->recordClass);
    printf("%s answer TTL: %u\n", funcName, dnsResourceRecord->ttl);
    printf("%s answer RDLength: %u\n", funcName, dnsResourceRecord->rdlength);
    printf("%s answer RData:", funcName);
    for (int i = 0; i < dnsResourceRecord->rdlength; ++i)
    {
        printf("%02x ", buffer[offset + i]);
    }

    printf("\n");
    size_t numOfBytesParsed = extractRecordAddress(buffer, getRecordType(dnsResourceRecord->type), offset, dnsResourceRecord->resourceData);
    printf("%s parsed %lu bytes, the answer as string:%s\n", funcName, numOfBytesParsed, dnsResourceRecord->resourceData);

    // Move past RData
    return offset + numOfBytesParsed;
}

int parseDnsResponse(IN const uint8_t* packet)
{
    const char funcName [] = "parseDnsResponse - ";
    if (NULL == packet)
    {
        printf("%s packet is NULL\n", funcName);
        return -1;
    }

    // Move the packet to the DNS payload section
    packet = packet + calculateOffsetToDnsPayload();

    //Parse the DNS header section
    DnsHeader* dnsHeader = (DnsHeader*)packet;
    uint16_t transactionId = ntohs(dnsHeader->id);
    uint16_t numOfQuestions = ntohs(dnsHeader->qdcount);
    uint16_t numOfAnswers = ntohs(dnsHeader->ancount);
    printf("%s transaction ID is:%u\n", funcName, transactionId);
    printf("%s number of questions:%u\n", funcName, numOfQuestions);
    printf("%s number of answers:%u\n", funcName, numOfAnswers);

    // Parse question
    // GuyA:It is assumed that the DNS response has only a single question.
    // If it has more, the function terminates without further processing.
    if (DNS_MAX_NUM_QUESTIONS_PER_RESPONSE < numOfQuestions)
    {
        printf("%s able to handle a DNS response with ONLY one question, but in this case there are %u, discarding the packet\n", funcName, numOfQuestions);
        return -2;
    }

    DnsQuestion dnsQuestion;
    memset(&dnsQuestion, 0, sizeof(dnsQuestion));
    packet += DNS_HEADER_SIZE;
    size_t offsetToAdd = parseDnsQuestion(packet, 0, &dnsQuestion);
    packet += offsetToAdd; // Forward buffer to the answers section

    // Parse answers
    // In this case, we support several answers 
    offsetToAdd = 0;
    for (uint8_t i = 0; i < numOfAnswers; ++i)
    {
        DnsResourceRecord dnsResourceRecord;
        memset(&dnsResourceRecord, 0, sizeof(dnsResourceRecord));
        offsetToAdd = parseDnsAnswer(packet, 0, &dnsResourceRecord);
        // GuyA: In the case of multiple answers instead of inserting each
        // address at a time, perhapse some kind of "aggragation" mechanism
        // might be more efficent (in particular is the DB is some sort of
        // entitiy in another machine, such as DB in AWS or some Redis DB)
        // CALL dataBaseMgrInsert()
        packet += offsetToAdd;
    }

    return 0;




    /*
    size_t offsetAfterParsingDnsQuestion = parseDnsQuestion((const uint8_t*) packet, dnsPlaloadOffset, &dnsQuestion);
    printf("%s offset returned is now:%lu from the begining of the entire frame\n", funcName, offsetAfterParsingDnsQuestion);
    const unsigned char *answer_section = dns_payload + sizeof(struct dns_header);
    printf("%s the DNS answer section part of the packet starts at address:%p and has the value:%02X\n", funcName, (void*)answer_section, *answer_section);
    return -1;

    // GuyA: This is where endianess can be tricky, and worth checking before
    // deployment
    int question_count = ntohs(dns->qdcount);

    // Skip over the question section
    for (int i = 0; i < question_count; i++) {
        // Skip over question name (variable length, terminated by 0 byte)
        while (*answer_section != 0) {
            answer_section++;
        }
        answer_section += 5; // Skip null byte and QTYPE, QCLASS (4 bytes)
    }

    // Process the answer section
    int answer_count = ntohs(dns->ancount);
    for (int i = 0; i < answer_count; i++) {
        // Skip over answer name (variable length, terminated by 0 byte)
        while (*answer_section != 0) {
            answer_section++;
        }
        answer_section++; // Skip null byte

        // Cast the remaining part of the answer to dns_rr struct
        struct dns_rr *rr = (struct dns_rr *)answer_section;
        uint16_t type = ntohs(rr->type);

        // Print record type
        printf("Record %d Type: %d ", i + 1, type);
        switch (type) {
            case 1: printf("(A)\n"); break;
            case 28: printf("(AAAA)\n"); break;
            case 5: printf("(CNAME)\n"); break;
            default: printf("(Unknown)\n"); break;
        }

        // Move to the next record (rdlength + 10 bytes for the DNS RR header)
        answer_section += 10 + ntohs(rr->rdlength);
    }
    */
}