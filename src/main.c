#include <stdio.h>

#include "parser/include/parser.h"
#include "utils/include/utils.h"

int main(int argc, char** argv)
{
    const char funcName [] = "main - ";
    printf("%s START\n", funcName);
    getCmdLineArgs(argc, argv);

    //int ret = sampleFuncUsingLibpcap();
    //printf("%s the sample function returned:%d\n", funcName, ret);
    
    printf("%s END\n", funcName);
    return 0;
}