#include <stdio.h>

#include "include/utils.h"

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
