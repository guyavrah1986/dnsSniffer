#include <stdio.h>

#include "include/utils.h"

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
        for (int i = 0; i < argc; i++)
        {
            printf("%s argv[%d]: %s\n", funcName, i, argv[i]);
        }
    }
}
