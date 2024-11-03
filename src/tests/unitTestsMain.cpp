#include <iostream>
#include <gtest/gtest.h>

// Local include files for the different objects we wish to test
// Each header file listed here contains GTest unit tests for some object(s)
#include "tests/testParser.h"
#include "tests/testUtils.h"
#include "tests/testDataBaseManager.h"
#include "tests/testHastTable.h"

using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	cout << "main - START of unit testing" << endl;
	int unitTestRetCode = RUN_ALL_TESTS();
	cout << "main - END of unit testing" << endl;
	return unitTestRetCode;
}
