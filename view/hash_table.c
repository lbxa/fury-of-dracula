//
// Created by eric on 16/7/20.
//

#include "hash_table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//djb2 hashing algorithm
int hash(const char *str, int table_size) {
    unsigned long hash = 5381;
    int c;

    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\0') break;
        c = str[i];
        hash = ((hash << 5) + hash) + c;
    }

    int hashIndex = hash % table_size;
    return hashIndex;
}

int equal(HashNode node, char *key) {
    return strcmp(node->key, key) == 0;
}

HashTable create_hash_table(int table_size) {
    HashTable table = malloc(sizeof(struct hash_table));
    assert(table != NULL);
    table->table_size = table_size;
    table->items = malloc(sizeof(struct hash_node) * table_size);
    assert(table->items != NULL);
    return table;
}

HashNode hash_get(HashTable table, char *key) {
    int hashVal = hash(key, table->table_size);

    if (table->items[hashVal] == NULL) {
        return NULL;
    } else {
        HashNode cur = table->items[hashVal];
        while (cur->head != NULL && !equal(cur, key)) {
            cur = cur->head;
        }
        // If equal then found
        if (equal(cur, key)) {
            return cur;
        }
        return NULL;
    }
}

void hash_insert(HashTable table, char *key, int value) {
    int hashVal = hash(key, table->table_size);
    HashNode newNode = malloc(sizeof(struct hash_node));
    newNode->head = NULL;
    newNode->value = value;
    newNode->key = malloc(sizeof(strlen(key)));
    strcpy(newNode->key, key);

    if (table->items[hashVal] == NULL) {
        table->items[hashVal] = newNode;
    } else {
        HashNode cur = table->items[hashVal];
        while (cur->head != NULL && !equal(cur, key)) {
            cur = cur->head;
        }
        if (!equal(cur, key)) {
            // Insert at end if not already in hashtable
            cur->head = newNode;
        } else {
            cur->value = value;
        }
    }
}

void hash_delete(HashTable table, char *key) {
    int hashVal = hash(key, table->table_size);

    if (table->items[hashVal] == NULL) {
        return;
    } else {
        HashNode cur = table->items[hashVal];
        HashNode last = NULL;
        while (cur->head != NULL && !equal(cur, key)) {
            last = cur;
            cur = cur->head;
        }
        // If equal then found
        if (equal(cur, key)) {
            if (last == NULL) {
                // Then this was at head of list
                table->items[hashVal] = cur->head;
            } else {
                last->head = cur->head;
            }
            free(cur);
        }
    }
}

void free_node(HashNode node) {
    free(node->key);
    free(node);
}

void drop_hash_table(HashTable ht) {
    for (int i = 0; i < ht->table_size; i++) {
        if (ht->items[i] != NULL) {
            HashNode cur = ht->items[i];
            while (cur) {
                HashNode next = cur->head;
                free_node(cur);
                cur = next;
            }
        }
    }
    free(ht);
}