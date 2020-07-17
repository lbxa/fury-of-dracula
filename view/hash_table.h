//
// Created by eric on 16/7/20.
//

#ifndef FURY_OF_DRACULA_HASH_TABLE_H
#define FURY_OF_DRACULA_HASH_TABLE_H

typedef struct hash_node {
    // Since dynamic sized arrays not allowed in course
    char *key;
    void *value;
    struct hash_node *head;
} *HashNode;

typedef struct hash_table {
    HashNode *items;
    int table_size;
    int num_items;
} *HashTable;

HashTable create_hash_table(int table_size);
HashNode hash_get(HashTable table, const char *key);
void hash_insert(HashTable table, const char *key, void *value);
void hash_delete(HashTable table, const char *key);
void drop_hash_table(HashTable table);
void print_hash_table(HashTable table, void (*print_node)(HashNode));

#endif //FURY_OF_DRACULA_HASH_TABLE_H
