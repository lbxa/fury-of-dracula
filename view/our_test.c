//
// Created by eric on 16/7/20.
//

#include "binary_heap.h"
#include "hash_table.h"
#include <stdio.h>

int main(void) {

    Heap *h = heap_create(32);
    HeapItem it = create_heap_item(3, "test");
    heap_push(h,it);
//    heap_push(h,5);
//    heap_push(h,3);
//    heap_push(h,7);
//    heap_push(h,9);
//    heap_push(h,8);
    heap_display(h);
    HeapItem v = heap_pop(h);
    printf("%d\n", v->value);

    HashTable table = create_hash_table(1024);
    hash_insert(table, "test", 0);
    hash_insert(table, "test", 3);
    hash_insert(table, "cool", 99);
    hash_insert(table, "fury", -12);

    HashNode a = hash_get(table, "test");
    HashNode b = hash_get(table, "cool");
    HashNode c = hash_get(table, "fury");

    printf("%s -> %d\n", a->key, a->value);
    printf("%s -> %d\n", b->key, b->value);
    printf("%s -> %d\n", c->key, c->value);

//    heap_display(h);
//    empty_heap(h);
    return 0;
}