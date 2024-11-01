#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "include/parser.h"
#include "../utils/include/utils.h"

#define DNS_PTR_NAME    0xc0

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
    //char qname[256];
    offset = parseQName(buffer, offset, dnsQuestion->question);
    
    uint16_t qtype = extract16(buffer, offset);
    uint16_t qclass = extract16(buffer, offset + 2);

    printf("Question Name: %s\n",  dnsQuestion->question);
    printf("Question Type: %u\n", qtype);
    printf("Question Class: %u\n", qclass);

    return offset + 4; // Move past QType and QClass
}

// Function to parse DNS answer section
size_t parseDNSAnswer(const uint8_t* buffer, size_t offset)
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

    uint16_t type = extract16(buffer, offset);
    uint16_t class = extract16(buffer, offset + 2);
    uint32_t ttl = extract32(buffer, offset + 4);
    uint16_t rdlength = extract16(buffer, offset + 8);
    offset += 10; // Move past Type, Class, TTL, and RDLength

    //printf("Answer Name: %s\n", name);
    printf("Answer Type: %u\n", type);
    printf("Answer Class: %u\n", class);
    printf("Answer TTL: %u\n", ttl);
    printf("Answer RDLength: %u\n", rdlength);

    printf("Answer RData: ");
    for (int i = 0; i < rdlength; i++)
    {
        printf("%02x ", buffer[offset + i]);
    }

    printf("\n");
    return offset + rdlength; // Move past RData
}
