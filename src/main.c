#include <stdio.h>

#include "utils/include/utils.h"
#include "listener/include/listener.h"

int componentTestEnterRunLoopAndCaptureSinglePacket()
{
    const char funcName [] = "componentTestEnterRunLoopAndCaptureSinglePacket - ";
    int ret = listenerPrepareToEnterRunLoop();
    if (0 != ret)
    {
        printf("%s the listenerPrepareToEnterRunLoop function was not successfull, aborting\n", funcName);
        return -1;
    }

    listenerRunLoop();
    ret = listenerCleanupAfterRunLoop();
    if (0 != ret)
    {
        printf("%s the listenerCleanupAfterRunLoop function was not successfull, aborting\n", funcName);
        return -1;
    }

    return ret;
}

int main(int argc, char** argv)
{
    const char funcName [] = "main - ";
    printf("%s ===START===\n", funcName);
    getCmdLineArgs(argc, argv);

    int ret = componentTestEnterRunLoopAndCaptureSinglePacket();
    printf("%s the componentTestEnterRunLoopAndCaptureSinglePacket function returned:%d\n", funcName, ret);

    
    printf("%s ===END===\n", funcName);
    return 0;
}