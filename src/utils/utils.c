#include <stdio.h>

#include "include/utils.h"

void displayDNSName(const uint8_t* questionName)
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

void printBytesAscii(const uint8_t* byteArr, const unsigned short numBytesToDisplay)
{
    for (unsigned short i = 0; i < numBytesToDisplay; ++i)
    {
        // Print each byte in hexadecimal format (2-digit uppercase hex)
        printf("%c ", byteArr[i]);
    }
}

void printBytes(const unsigned char* byteArr, const unsigned short numBytesToDisplay)
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

void getCmdLineArgs(int argc, char** argv)
{
    const char funcName [] = "getCmdLineArgs - ";
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
