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
int8_t hash(IN const char* key);
hash_table* create_table();
struct node* create_list_node(const char* value);
void add_to_list(struct node** list, const char* value);
void insert(hash_table* table, const char* key, const char* value);
struct node* find(hash_table* table, const char* key);
void display_list(struct node* list);
void display_table(hash_table* table);
void free_list(struct node* list);
void free_table(hash_table* table);
