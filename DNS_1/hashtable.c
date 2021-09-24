#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <string.h>

#include "hashtable.h"

HashTableCell* CreateHashTableCell(DomainName domainName, IPADDRESS ip)
{
    HashTableCell* cell = (HashTableCell*)calloc(1, sizeof(HashTableCell));
    cell->ip = ip;
    cell->domainName = (DomainName)calloc(strlen(domainName) + 1, sizeof(char));
    strcpy(cell->domainName, domainName);
    cell->next = NULL;
    return cell;
}


size_t HashStringFAQ6(DomainName domainName)
{
    size_t hash = 0;

    for (; *domainName; domainName++)
    {
        hash += (unsigned char)(*domainName);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}


HashTable* InitHashTable(size_t size)
{
    HashTable* table = (HashTable*)calloc(1, sizeof(HashTable));

    if (table == NULL)
    {
        return INVALID_DNS_HANDLE;
    }

    table->size = size;
    table->pCells = (HashTableCell**)calloc(size, sizeof(HashTableCell*));

    if (table->pCells == NULL)
    {
        free(table);
        return INVALID_DNS_HANDLE;
    }

    for (size_t i = 0; i < table->size; i++)
    {
        table->pCells[i] = NULL;
    }
    return table;
}


void DestroyHashTable(HashTable** pTable)
{
    HashTable* table = *pTable;
    for (size_t i = 0; i < table->size; i++)
    {
        HashTableCell* pCell = table->pCells[i], * pDelCell;
        while (pCell != NULL)
        {
            pDelCell = pCell;
            pCell = pCell->next;
            free(pDelCell);
        }
        table->pCells[i] = NULL;
    }
    table->size = 0;
    free(table->pCells);
    free(table);
    *pTable = NULL;
}


void AddInHashTable(HashTable* table, DomainName domainName, IPADDRESS ip)
{
    size_t hashIndex = HashStringFAQ6(domainName) % table->size;

    for (HashTableCell* pCell = table->pCells[hashIndex]; pCell != NULL; pCell = pCell->next)
    {
        if (strcmp(pCell->domainName, domainName) == 0)
        {
            pCell->ip = ip;
            return;
        }
    }

    HashTableCell* cell = CreateHashTableCell(domainName, ip);
    cell->next = table->pCells[hashIndex];
    table->pCells[hashIndex] = cell;
}


IPADDRESS GetFromHashTable(HashTable* table, DomainName domainName)
{
    size_t hashIndex = HashStringFAQ6(domainName) % table->size;

    for (HashTableCell* pCell = table->pCells[hashIndex]; pCell != NULL; pCell = pCell->next)
    {
        if (strcmp(pCell->domainName, domainName) == 0)
        {
            return pCell->ip;
        }
    }
    return INVALID_IP_ADDRESS;
}
