#include <iostream>
#include <gtest/gtest.h>

// Local include files for the different objects we wish to test
// Each header file listed here contains GTest unit tests for some objects
#include "tests/testParser.h"
#include "tests/testUtils.h"
#include "tests/testDataBaseManager.h"
#include "tests/testHastTable.h"

using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	// The first paramter is the path to the logger configuration file
	
	cout << "main - start of unit testing" << endl;
	int unitTestRetCode = RUN_ALL_TESTS();
	return unitTestRetCode;
}
