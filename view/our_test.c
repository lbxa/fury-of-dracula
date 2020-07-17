//
// Created by eric on 16/7/20.
//

#include "binary_heap.h"
#include "hash_table.h"
#include "Map.h"
#include <stdio.h>
#include <time.h>
#include "path_finding.h"

void print_hash_node(HashNode node) {
    Path p = (Path) node->value;
    printf("[%s -> %d]\n", p->place, p->distance);
}

int main(void) {

    Heap h = heap_create(32);
    HeapItem it = create_heap_item(3, "test");
    heap_push(h,it);
    heap_display(h);
    HeapItem v = heap_pop(h);
    printf("%d\n", v->value);

    Map map = MapNew();
    clock_t start = clock();
    HashTable distances;
    HashTable *paths_lookup = getAllPossiblePaths(map);
    HashNode path = hash_get(paths_lookup[PLACES[2].id], PLACES[50].abbrev);
    printf("%s -> %d\n", path->key, ((Path) path->value)->distance);
    clock_t end = clock();
    print_hash_table(paths_lookup[0], print_hash_node);

    printf("%lf\n", (double) (end-start)/CLOCKS_PER_SEC);

//    heap_display(h);
//    empty_heap(h);
    return 0;
}