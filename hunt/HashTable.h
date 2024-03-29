//
// Hash table implementation using seperate chaining to resolve collisions
//

#ifndef FURY_OF_DRACULA_HASH_TABLE_H
#define FURY_OF_DRACULA_HASH_TABLE_H

typedef struct hash_node {
  char *key;
  void *value;
  struct hash_node *head;
} * HashNode;

typedef struct HashTable {
  HashNode *items;
  int tableSize;
  int numItems;
} * HashTable;

/**
 * Creates a new hash table for the given table size
 * @param initial_capacity
 * @return
 */
HashTable HashTableCreate(int tableSize);

/**
 * Gets the HashNode for the given key in the table
 * @param table
 * @param key
 */
HashNode HashGet(HashTable table, const char *key);

/**
 * Inserts a given key with given value to the hash table
 * @param table
 * @param key
 */
void HashInsert(HashTable table, const char *key, void *value);

/**
 * Deletes a given key from the given hash table
 * @param table
 * @param key
 */
void HashDelete(HashTable table, const char *key);

/**
 * Frees all associated memory for a given hash table
 * @param table
 */
void HashTableDestroy(HashTable table, void (*nodeFreeFunction)(void *));

/**
 * Prints a given hash table to stdout
 * @param table
 * @param printNode
 */
void HashTableDisplay(HashTable table, void (*printNode)(HashNode));

#endif  // FURY_OF_DRACULA_HASH_TABLE_H
