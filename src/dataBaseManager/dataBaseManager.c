#include <stdio.h>

#include "include/dataBaseManager.h"
#include "../utils/include/hashTable.h"

// This is the global (per translation unit) hash table.
// There should be only one such table throught the entire lifetime of 
// the program.
static hash_table* g_table;

int dataBaseMgrInit()
{
    const char funcName [] = "dataBaseMgrInit -";
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

    insert(g_table, key, dnsRecordToAdd->resourceData);
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

    return find(g_table, key);
}

void dataBaseMgrClean()
{
    const char funcName [] = "dataBaseMgrClean -";
    free_table(g_table);
    printf("%s hash table was free-ed successfully\n", funcName);
}