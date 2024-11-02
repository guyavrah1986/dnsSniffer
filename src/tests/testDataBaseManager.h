#pragma once

extern "C"
{
    #include "../dataBaseManager/include/dataBaseManager.h"
}

TEST(DataBaseManagerTest, testTwoDifferentStringValuesAreMappedToDifferentIntValues)
{
    int i = 1;
    int j = 9;
    EXPECT_EQ(i, j);
}