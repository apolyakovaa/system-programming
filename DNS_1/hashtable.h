#ifndef _LAB_1_HASHTABLE_H_
#define _LAB_1_HASHTABLE_H_

#include "dns.h"

typedef char* DomainName;

/** Hash table cell for storing data. **/
typedef struct HashTableCell
{
    DomainName        domainName;
    IPADDRESS                 ip;
    struct HashTableCell* next;
} HashTableCell;


/** Creates hash table cell. **/
HashTableCell* CreateHashTableCell(DomainName domainName, IPADDRESS ip);


/** Hash table allow insert, get key for O(1)
 * size: size_t            - size of hash
 * tablepCells: HashTableCell** - pointer on cells of hash table **/
typedef struct HashTable
{
    size_t           size;
    HashTableCell** pCells;
} HashTable;


/** FAQ6 hash function **/
size_t HashStringFAQ6(DomainName domainName);


/** Initializes hash table **/
HashTable* InitHashTable(size_t size);


/** Free allocated memory **/
void DestroyHashTable(HashTable** pTable);


/** Add new domainName:ip pair to hash table **/

void AddInHashTable(HashTable* table, DomainName domainName, IPADDRESS ip);


/** Find and return value by key form hash table **/
IPADDRESS GetFromHashTable(HashTable* table, DomainName domainName);



#endif