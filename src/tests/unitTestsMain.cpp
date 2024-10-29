#include <iostream>
#include <gtest/gtest.h>

// Local include files for the different objects we wish to test
// Each header file listed here contains GTest unit tests for some objects
#include "tests/testParser.h"

using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	// The first paramter is the path to the logger configuration file
	
	cout << "main - start of unit testing" << endl;

	// This line will configuer the root logger for the entire
	// application and/or playground flow
	int unitTestRetCode = RUN_ALL_TESTS();
	return unitTestRetCode;
}
