#pragma once

extern "C"
{
    #include "../utils/include/utils.h"    
}

TEST(UtilsTests, sanityTest)
{ 
	EXPECT_EQ(0, 0);
    int argc = 1;
    char** argv = NULL;
    utilsGetCmdLineArgs(argc, argv);
}
