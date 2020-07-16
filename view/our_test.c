//
// Created by eric on 16/7/20.
//

#include "binary_heap.h"
#include "hash_table.h"
#include "Map.h"
#include <stdio.h>
#include <time.h>

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
    hash_insert(table, "abc", 99);
    hash_insert(table, "def", -12);

    HashNode a = hash_get(table, "test");
    HashNode b = hash_get(table, "abc");
    HashNode c = hash_get(table, "def");

    printf("%s -> %d\n", a->key, a->value);
    printf("%s -> %d\n", b->key, b->value);
    printf("%s -> %d\n\n", c->key, c->value);

    Map map = MapNew();
    clock_t start = clock();
    HashTable distances;
    HashTable *paths_lookup = getAllPossiblePaths(map);
    HashNode path = hash_get(paths_lookup[PLACES[2].id], PLACES[50].abbrev);
    printf("%s -> %d\n", path->key, path->value);
    clock_t end = clock();

    printf("%lf\n", (double) (end-start)/CLOCKS_PER_SEC);

//    heap_display(h);
//    empty_heap(h);
    return 0;
}