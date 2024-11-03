#pragma once

extern "C"
{
    #include "../utils/include/hashTable.h"
}

TEST(HashTableTest, testSanityCreationOfHashTable)
{
    hash_table* table = create_table();
    ASSERT_NE(table, nullptr);

    // Insert values into the table
    insert(table, "google.com", "8.8.8.8");
    insert(table, "google.com", "142.251.142.206");
    insert(table, "yahoo.com", "2.2.2.2");

    // Display the table
    printf("Hash table contents:\n");
    display_table(table);

    // Find and display values for a specific key
    const char valToSearch [] = "google.com";
    struct node* list = find(table, valToSearch);
    ASSERT_NE(list, nullptr);
    printf("Values for the value to search:%s: \n", valToSearch);
    display_list(list);

    // Free the hash table
    free_table(table);
}

TEST(HashTableTest, testSanityCreationOfHashTableAndOneCollision)
{
    hash_table* table = create_table();
    ASSERT_NE(table, nullptr);

    // Insert values into the table
    insert(table, "google.com", "8.8.8.8");
    insert(table, "google.com", "142.251.142.206");
    insert(table, "yahoo.com", "2.2.2.2");

    // Display the table
    printf("Hash table contents:\n");
    display_table(table);

    // Find and display values for a specific key
    struct node* list = find(table, "google.com");
    ASSERT_NE(list, nullptr);
    printf("Values for google.com: \n");
    display_list(list);

    // Make the collision:
    insert(table, "yhooa.com", "3.3.3.3");
    printf("after making a collision the hash table contents:\n");
    display_table(table);

    // Find and display values for the collied value
    list = find(table, "yhooa.com");
    ASSERT_NE(list, nullptr);
    printf("Values for yhooa.com: \n");
    display_list(list);

    list = find(table, "yahoo.com");
    ASSERT_NE(list, nullptr);
    printf("Values for yahoo.com: \n");
    display_list(list);

    // Add additional value for one of the "collided" keys
    insert(table, "yhooa.com", "4.4.4.4");
    printf("after adding second value for one of the collided keys, the hash table contents:\n");
    display_table(table);

    // Free the hash table
    free_table(table);
}

TEST(HashTableTest, testFindNonExistingElementInTheHashTable)
{
    hash_table* table = create_table();
    ASSERT_NE(table, nullptr);

    // Insert values into the table
    insert(table, "google.com", "8.8.8.8");
    insert(table, "google.com", "142.251.142.206");
    insert(table, "yahoo.com", "2.2.2.2");

    // Display the table
    printf("Hash table contents:\n");
    display_table(table);

    // Find and display values for a specific key
    struct node* list = find(table, "cnn.com");
    EXPECT_EQ(list, nullptr);

    // Free the hash table
    free_table(table);
}

TEST(HashTableTest, testAdditionOfAddressToAnExistingKey)
{
    hash_table* table = create_table();

    // Insert values into the table
    insert(table, "google.com", "8.8.8.8");
    insert(table, "google.com", "142.251.142.206");
    insert(table, "yahoo.com", "2.2.2.2");

    // Display the table
    printf("Hash table contents:\n");
    display_table(table);

    // Find and display values for a specific key
    insert(table, "yahoo.com", "3.3.3.3");
    struct node* list = find(table, "yahoo.com");
    ASSERT_NE(list, nullptr);
    printf("Hash table contents after addition to yahoo.com:\n");
    display_table(table);

    // Try to insert the SAME value that already exists for some key
    insert(table, "google.com", "8.8.8.8");
    printf("Hash table contents after trying to insert duplicated value:%s to key:%s:\n", "8.8.8.8", "google.com");
    display_table(table);

    // Free the hash table
    free_table(table);
}


// GuyA: Add the following test cases:
// 1.Removal of elements from the hash table