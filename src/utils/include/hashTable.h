#pragma once

#include <stdint.h>

#include "utils.h"

// Global definitions for this module:
#define HASH_TABLE_SIZE 100 

// Define the hash table entry
typedef struct hash_entry
{
    const char* key;
    struct node* value;        // Linked list as the value
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
int8_t getHash(IN const char* key);
hash_table* create_table();
struct node* create_list_node(IN const char* value);
void add_to_list(IN struct node** list, IN const char* value);
void insert(IN hash_table* table, IN const char* key, IN const char* value);
struct node* find(IN hash_table* table, IN const char* key);
void display_list(IN struct node* list);
void display_table(IN hash_table* table);
void free_list(IN struct node* list);
void free_table(IN hash_table* table);
