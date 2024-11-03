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

TEST(DataBaseManagerTest, testCreationAndDestructionOfTheHashTable)
{
    int retVal = dataBaseMgrInit();
    EXPECT_EQ(0, retVal);

    // Try invalid insertion
    const char key [] = "google.com";
    int ret = dataBaseMgrInsertItem(key, NULL);
    EXPECT_EQ(1, ret);

    // Insert a concrete key,value to an empty DB (hash table)
    DnsResourceRecord dnsRecordToAdd;
    memset(&dnsRecordToAdd, 0, sizeof(dnsRecordToAdd));
    dnsRecordToAdd.name = 0x1;
    dnsRecordToAdd.ttl = 140;
    dnsRecordToAdd.type = 1;
    dnsRecordToAdd.rdlength = PARSER_IPv4_ADDR_LEN;
    dnsRecordToAdd.recordClass = 1;
    const uint8_t addr [] = {0x8e, 0xfa, 0x4b, 0x2e};
    size_t offset = 0;
    sprintf(dnsRecordToAdd.resourceData, "%u.%u.%u.%u", addr[offset], addr[offset + 1], addr[offset + 2], addr[offset + 3]);
    printf("dnsRecordToAdd.resourceData is:%s\n", dnsRecordToAdd.resourceData);

    // Verify insertion
    ret = dataBaseMgrInsertItem(key, &dnsRecordToAdd);
    EXPECT_EQ(0, ret);

    // Check getter of the DB returns list of single item that was inserted 
    // eariler to the DB
    struct node* itemList = dataBaseMgrGetItem(key);
    ASSERT_NE(itemList, nullptr);

    // Make sure the returned list is correct, there is only one item and
    // its value is known, so check that it is indeed the case
    EXPECT_STREQ(itemList->val, dnsRecordToAdd.resourceData);

    // Clean DB
    dataBaseMgrClean();
}
