#include <stdio.h>
#include <pthread.h>

#include "include/dataBaseManager.h"
#include "../utils/include/hashTable.h"

// This is the global (per translation unit) hash table.
// There should be only one such table throught the entire lifetime of 
// the program.
static hash_table* g_table;

// Used to share the DB with the UI thread for synchronized access
pthread_mutex_t g_dataBaseMutex = PTHREAD_MUTEX_INITIALIZER;

int dataBaseMgrInit()
{
    const char funcName [] = "dataBaseMgrInit -";
    if (0 != pthread_mutex_init(&g_dataBaseMutex, NULL))
    {
        printf("%s mutex initialization failed\n", funcName);
        return 1;
    }

    g_table = create_table();
    if (NULL != g_table)
    {
        printf("%s created hash table successfully\n", funcName);
        return 0;
    }

    printf("%s was unable to created hash table successfully, need to abort\n", funcName);
    return 1;
}

int dataBaseMgrInsertItem(IN const char* key, IN const DnsResourceRecord* dnsRecordToAdd)
{
    const char funcName [] = "dataBaseMgrInsertItem - ";
    if (NULL == key || NULL == dnsRecordToAdd)
    {
        printf("%s dnsRecordToAdd and/or key is NULL\n", funcName);
        return 1;
    }

    // GuyA: Lock writing
    pthread_mutex_lock(&g_dataBaseMutex);
    insert(g_table, key, dnsRecordToAdd->resourceData);
    pthread_mutex_unlock(&g_dataBaseMutex);
    return 0;
}

struct node* dataBaseMgrGetItem(IN const char* key)
{
    const char funcName [] = "dataBaseMgrGetItem - ";
    if (NULL == key)
    {
        printf("%s got key which is NULL\n", funcName);
        return NULL;
    }

    // GuyA: If we add here copy of the returned list, then the lock in the
    // case of reading can end here and not later in a "larger scope" that 
    // increments the critical section
    return find(g_table, key);
}

void dataBaseMgrClean()
{
    const char funcName [] = "dataBaseMgrClean -";
    pthread_mutex_destroy(&g_dataBaseMutex);
    free_table(g_table);
    printf("%s hash table was free-ed successfully\n", funcName);
}