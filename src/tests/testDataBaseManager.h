#pragma once

extern "C"
{
    #include "../dataBaseManager/include/dataBaseManager.h"
    #include "../utils/include/hashTable.h"
}

TEST(DataBaseManagerTest, testTwoDifferentStringValuesAreMappedToDifferentIntValues)
{
    const char googleComStrName [] = "google.com";
    uint32_t googleComHashVal = getHash(googleComStrName);
    const char cnnComStrName [] = "cnn.com";
    uint32_t cnnComHashVal = getHash(cnnComStrName);
    ASSERT_NE(googleComHashVal, cnnComHashVal);
}

TEST(DataBaseManagerTest, testTwoStringsWithTheSameValuesAreMappedToTheSameValue)
{
    const char googleComStrName1 [] = "google.com";
    uint32_t googleComHashVal1 = getHash(googleComStrName1);
    const char googleComStrName2 [] = "google.com";
    uint32_t googleComHashVal2 = getHash(googleComStrName2);
    ASSERT_EQ(googleComHashVal1, googleComHashVal2);
}

TEST(DataBaseManagerTest, testCreateLinkedListWithTwoNodes)
{
    struct node* head = NULL;

    // Create and add the first node
    const char value1 [] = "8.8.8.8";
    insert_end(&head, value1);

    // Create the second node
    const char value2 [] = "142.251.142.206";
    insert_end(&head, value2);

    //display_list(head);
}

TEST(DataBaseManagerTest, testFindNodeInLinkedListWithTwoNodes)
{
    struct node* head = NULL;

    // Create and add the first node
    const char value1 [] = "8.8.8.8";
    insert_end(&head, value1);
    EXPECT_NE(head, nullptr);
    display_list(head);

    // Create the second node
    const char value2 [] = "142.251.142.206";
    insert_end(&head, value2);
    EXPECT_STREQ(head->val, value1);

    display_list(head);
    // Find first element
    struct node* ret = find_node(head, value1);
    EXPECT_NE(ret, nullptr);
    ret = find_node(head, value2);
    EXPECT_NE(ret, nullptr);
}