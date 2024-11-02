#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arpa/inet.h>

#include "include/parser.h"
#include "../utils/include/utils.h"

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

static size_t calculateOffsetToDnsPayload()
{
    const char funcNmae [] = "parser::calculateOffsetToDnsPayload - ";
    size_t offsetToDnsPayload = 0;

    // GuyA:According to some research done online, it appears that when a WiFi 
    // packet (802.11x) is being handed from the driver to the rest of the
    // network stack, it is being "considered" as a "fake" Ethernet (802.3) 
    // frame. This make sense, becuase WiFi was developed to be the "wireless
    // Ethernet" --> so in any case, assume the Link layer header is an 
    // Ethernet frame --> its length is 14 bytes
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

// Function to parse QName (domain name)
size_t parseQName(IN const uint8_t* buffer, IN size_t offset, OUT char* qname)
{
    size_t pos = offset;
    size_t name_index = 0;
    while (buffer[pos] != 0)
    {
        int label_len = buffer[pos++];
        for (int i = 0; i < label_len; i++)
        {
            qname[name_index++] = buffer[pos++];
        }
        
        qname[name_index++] = '.'; // Separate labels with '.'
    }

    qname[name_index - 1] = '\0'; // Null-terminate the string and remove last '.'
    return pos + 1; // Position after QName (0 terminator)
}

// Function to parse DNS question section
size_t parseDnsQuestion(IN const uint8_t* buffer, IN size_t offset, OUT DnsQuestion* dnsQuestion)
{
    offset = parseQName(buffer, offset, dnsQuestion->question);
    uint16_t qtype = extract16(buffer, offset);
    uint16_t qclass = extract16(buffer, offset + 2);

    printf("Question Name: %s\n",  dnsQuestion->question);
    printf("Question Type: %u\n", qtype);
    printf("Question Class: %u\n", qclass);

    return offset + 4; // Move past QType and QClass
}

// Function to parse DNS answer section
size_t parseDnsAnswer(IN const uint8_t* buffer, IN size_t offset, OUT DnsResourceRecord* dnsResourceRecord)
{
    // NOTE: The assumption here is that the name field in the answer will be
    // of type DNS_PTR_NAME (0xc0) --> so it means that the next byte is the 
    // offset from the begining of the DNS packet, and then there are the 
    // rest of the fields of the DNS answer
    
    if (DNS_PTR_NAME != buffer[offset])
    {
        printf("currently only answers with type DNS_PTR_NAME are supported, and this is not the case - aborting parse\n");
        return 0;
    }
    
    // char name[256];
    //offset = parseQName(buffer, offset, name); // Handle name (could use compression)
    offset += 2;
    printf("offset from DNS packet begining now is:%lu\n", offset);

    dnsResourceRecord->type = extract16(buffer, offset);
    dnsResourceRecord->recordClass = extract16(buffer, offset + 2);
    dnsResourceRecord->ttl = extract32(buffer, offset + 4);
    dnsResourceRecord->rdlength = extract16(buffer, offset + 8);
    offset += 10; // Move past Type, Class, TTL, and RDLength

    //printf("Answer Name: %s\n", name);
    printf("Answer Type: %u\n", dnsResourceRecord->type);
    printf("Answer Class: %u\n", dnsResourceRecord->recordClass);
    printf("Answer TTL: %u\n", dnsResourceRecord->ttl);
    printf("Answer RDLength: %u\n", dnsResourceRecord->rdlength);
    printf("Answer RData: ");
    for (int i = 0; i < dnsResourceRecord->rdlength; ++i)
    {
        printf("%02x ", buffer[offset + i]);
    }

    // GuyA: for now, assume address is a legitimate IPv4 address
    sprintf(dnsResourceRecord->resourceData, "%u.%u.%u.%u", buffer[offset], buffer[offset + 1], buffer[offset + 2], buffer[offset + 3]);
    printf("\n");
    return offset + dnsResourceRecord->rdlength; // Move past RData
}

int parseDnsResponse(IN const uint8_t* packet)
{
    const char funcName [] = "parseDnsResponse - ";

    size_t dnsPlaloadOffset = calculateOffsetToDnsPayload();
    printf("%s DNS payload is in offset of:%lu from the begining of the frame\n", funcName, dnsPlaloadOffset);
    packet = packet + dnsPlaloadOffset;

    // 1. Parse the DNS header section
    DnsQuestion dnsQuestion;
    memset(&dnsQuestion, 0, sizeof(dnsQuestion));
    DnsHeader* dnsHeader = (DnsHeader*)packet;
    uint16_t numOfQuestions = ntohs(dnsHeader->qdcount);
    printf("%s number of questions:%u\n", funcName, numOfQuestions);

    // 2. Parse questions
    // packet += DNS_HEADER_SIZE;
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