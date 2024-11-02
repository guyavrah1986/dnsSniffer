#pragma once

extern "C"
{
    #include "../dataBaseManager/include/dataBaseManager.h"
    #include "../utils/include/utils.h"
}

TEST(DataBaseManagerTest, testTwoDifferentStringValuesAreMappedToDifferentIntValues)
{
    const char googleComStrName [] = "google.com";
    uint32_t googleComHashVal = utilsHash(googleComStrName);
    const char cnnComStrName [] = "cnn.com";
    uint32_t cnnComHashVal = utilsHash(cnnComStrName);
    ASSERT_NE(googleComHashVal, cnnComHashVal);
}

TEST(DataBaseManagerTest, testTwoStringsWithTheSameValuesAreMappedToTheSameValue)
{
    const char googleComStrName1 [] = "google.com";
    uint32_t googleComHashVal1 = utilsHash(googleComStrName1);
    const char googleComStrName2 [] = "google.com";
    uint32_t googleComHashVal2 = utilsHash(googleComStrName2);
    ASSERT_EQ(googleComHashVal1, googleComHashVal2);
}