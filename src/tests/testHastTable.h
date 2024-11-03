#pragma once

extern "C"
{
    #include "../utils/include/hashTable.h"
}

TEST(HashTableTest, testSanityCreationOfHashTable)
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
    struct node* list = find(table, "google.com");
    printf("Values for google.com: \n");
    display_list(list);

    // Free the hash table
    free_table(table);
}

TEST(HashTableTest, testSanityCreationOfHashTableAndOneCollision)
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
    struct node* list = find(table, "google.com");
    printf("Values for google.com: \n");
    display_list(list);

    // Make the collision:
    insert(table, "yhooa.com", "3.3.3.3");
    printf("after making a collision the hash table contents:\n");
    display_table(table);

    // Free the hash table
    free_table(table);
}

TEST(HashTableTest, testFindNonExistingElementInTheHashTable)
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
    struct node* list = find(table, "cnn.com");
    EXPECT_EQ(list, nullptr);

    // Free the hash table
    free_table(table);
}

// GuyA: Add the following test cases:
// 1.Removal of elements from the hash table