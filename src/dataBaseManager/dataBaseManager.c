#include <stdio.h>

#include "../utils/include/hashTable.h"

// This is the global (per translation unit) hash table.
// There should be only one such table throught the entire lifetime of 
// the program.
static hash_table* table;

int dataBaseMgrInit()
{
    const char funcName [] = "dataBaseMgrInit -";
    table = create_table();
    if (NULL != table)
    {
        printf("%s created hash table successfully\n", funcName);
        return 0;
    }

    printf("%s was unable to created hash table successfully, need to abort\n", funcName);
    return 1;
}

void dataBaseMgrClean()
{
    const char funcName [] = "dataBaseMgrClean -";
    free_table(table);
    printf("%s hash table was free-ed successfully\n", funcName);
}