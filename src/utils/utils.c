#include <stdio.h>

#include "include/utils.h"

uint32_t utilsHash(IN const char* str)
{
    uint32_t hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
        
    return hash;
}

void utilsdisplayDnsName(IN const uint8_t* questionName)
{
    uint8_t len = questionName[0];  // First byte is the length of the first label
    int pos = 1;
    printf("len is:%u\n", len);

    while (len > 0 && pos < 255) { // DNS_QNAME_MAX_LEN
        for (int i = 0; i < len; i++) {
            printf("%c", questionName[pos + i]);
        }

        pos += len; // Move past this label
        len = questionName[pos]; // Next label length
        pos++;

        if (len > 0) {
            printf("."); // Print the dot between segments
        }
    }
    printf("\n");
}

void utilsPrintBytesAscii(IN const uint8_t* byteArr, IN const unsigned short numBytesToDisplay)
{
    for (unsigned short i = 0; i < numBytesToDisplay; ++i)
    {
        // Print each byte in hexadecimal format (2-digit uppercase hex)
        printf("%c ", byteArr[i]);
    }
}

void printBytes(IN const unsigned char* byteArr, IN const unsigned short numBytesToDisplay)
{
    const char funcName [] = "printBytes - ";
    printf("%s about to display %u bytes that starts at address:%p\n", funcName, numBytesToDisplay, (void*)byteArr);
    for (unsigned short i = 0; i < numBytesToDisplay; ++i)
    {
        // Print each byte in hexadecimal format (2-digit uppercase hex)
        printf("%02X ", byteArr[i]);
    }

    printf("\n");
}

void utilsGetCmdLineArgs(IN int argc, IN char** argv)
{
    const char funcName [] = "utilsGetCmdLineArgs - ";
    printf("%s got %d arguments from command line\n", funcName, argc);
    if (argc == 1)
    {
        // No arguments were passed (only the program name is present)
        printf("%s no arguments were passed\n", funcName);
    } 
    else
    {
        // Print all arguments, including the program name
        printf("Arguments:\n");
        for (int i = 0; i < argc; ++i)
        {
            printf("%s argv[%d]: %s\n", funcName, i, argv[i]);
        }
    }
}
