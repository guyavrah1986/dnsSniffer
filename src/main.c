#include "utils/include/utils.h"
#include "systemManager/include/systemManager.h"

int main(int argc, char** argv)
{
    const char funcName [] = "main - ";
    printf("%s ===START===\n", funcName);
    utilsGetCmdLineArgs(argc, argv);

    int ret = systemMgrRunProgram();
    printf("%s the systemMgrRunProgram function returned:%d\n", funcName, ret);

    printf("%s ===END===\n", funcName);
    return ret;
}