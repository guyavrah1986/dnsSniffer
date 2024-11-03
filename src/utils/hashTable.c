#include "include/hashTable.h"

int8_t getHash(IN const char* key)
{
    if (NULL == key)
    {
        return -1;
    }

    unsigned long int hash = 0;
    int i = 0;
    while (key[i] != '\0')
    {
        hash = (hash << 8) + key[i];
        i++;
    }

    return hash % HASH_TABLE_SIZE;
}

// Initialize the hash table
hash_table* create_table()
{
    hash_table* table = malloc(sizeof(hash_table));
    if (NULL == table)
    {
        return NULL;
    }

    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        table->entries[i] = NULL;
    }

    return table;
}

// Create a new linked list node with a string value
struct node* create_list_node(IN const char* value)
{
    struct node* new_node = malloc(sizeof(struct node));
    if (NULL == new_node)
    {
        return NULL;
    }
    
    new_node->val = strdup(value);
    new_node->next = NULL;
    return new_node;
}

// Add a value to the linked list
void add_to_list(IN struct node** list, IN const char* value)
{
    const char funcName [] = "add_to_list -";
    if (NULL == value)
    {
        printf("%s value providied is NULL\n", funcName);
        return;
    }

    struct node* new_node = create_list_node(value);
    if (NULL == new_node)
    {
        printf("%s the node created is NULL, not inserting new element for value:%s\n", funcName, value);
        return;
    }

    new_node->next = *list;
    *list = new_node;
}

// Insert a key-value pair into the hash table
void insert(IN hash_table* table, IN const char* key, IN const char* value)
{
    const char funcName [] = "insert -";
    if (NULL == key || NULL == value)
    {
        printf("%s key and/or value is(are) NULL\n", funcName);
        return;
    }

    unsigned int index = getHash(key);
    hash_entry* entry = table->entries[index];
    // Check if the key already exists
    while (NULL != entry)
    {
        if (0 == strcmp(entry->key, key))
        {
            // Key exists, add value to the linked list
            add_to_list(&entry->value, value);
            return;
        }

        entry = entry->next;
    }

    // Key does not exist, create a new entry
    entry = malloc(sizeof(hash_entry));
    entry->key = strdup(key);
    entry->value = NULL;
    entry->next = table->entries[index];
    table->entries[index] = entry;

    // Insert the value into the linked list
    add_to_list(&entry->value, value);
}

// Find the linked list associated with a key
struct node* find(IN hash_table* table, IN const char* key)
{
    unsigned int index = getHash(key);
    hash_entry* entry = table->entries[index];

    // Traverse the chain to find the key
    while (NULL != entry)
    {
        if (0 == strcmp(entry->key, key))
        {
            return entry->value;
        }

        entry = entry->next;
    }

    return NULL;  // Key not found
}

// Display the linked list
void display_list(IN struct node* list)
{
    struct node* current = list;
    while (NULL != current)
    {
        printf("%s -> ", current->val);
        current = current->next;
    }

    printf("NULL\n");
}

// Display the entire hash table
void display_table(IN hash_table* table)
{
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        hash_entry* entry = table->entries[i];
        if (NULL != entry)
        {
            printf("Index %d:\n", i);
            while (NULL != entry)
            {
                printf("  Key: %s, Values: ", entry->key);
                display_list(entry->value);
                entry = entry->next;
            }
        }
    }
}

// Free the linked list
void free_list(IN struct node* list)
{
    struct node* current = list;
    while (NULL != current)
    {
        struct node* temp = current;
        current = current->next;
        free(temp->val);
        free(temp);
    }
}

// Free the hash table
void free_table(IN hash_table* table) 
{
    for (int i = 0; i < HASH_TABLE_SIZE; ++i)
    {
        hash_entry* entry = table->entries[i];
        while (NULL != entry)
        {
            hash_entry* temp = entry;
            entry = entry->next;

            free((char*)temp->key);
            free_list(temp->value);
            free(temp);
        }
    }
    
    free(table);
}

