#include <stdint.h>

#include "utils.h"

// Global definitions for this module:
#define HASH_TABLE_SIZE 100 

// Define the hash table entry
typedef struct hash_entry
{
    const char* key;
    struct node* value;  // Linked list as the value
    struct hash_entry* next;  // For handling collisions with chaining
} hash_entry;

// Define the hash table structure
typedef struct hash_table
{
    hash_entry* entries[HASH_TABLE_SIZE];
} hash_table;

// ==============
// Hash table API
// ==============

/*
Hashing function to retrieve the index to which the key will be inserted
in the array of hash_entry's.
*/
//int8_t hash(IN const char* key);