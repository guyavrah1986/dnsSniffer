#include "include/systemManager.h"
#include "../dataBaseManager/include/dataBaseManager.h"

int systemMgrInit()
{
    const char funcName [] = "systemMgrInit - ";
    int ret = dataBaseMgrInit();
    if (0 != ret)
    {
        printf("%s was unable to initialize the system correctly, aborting\n", funcName);
        return 1;
    }

    return 0;
}

void systemMgrTerminate()
{
    dataBaseMgrClean();
}