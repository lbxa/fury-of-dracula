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

void print_heap_item(HeapItem node) {
    Path p = (Path) node->data;
    printf("[%s -> %d]\n", p->place, p->distance);
}

int main(void) {

    Map map = MapNew();
    clock_t start = clock();
//    HashTable distances;
    HashTable *paths_lookup = GetAllPathLookup(map);
    HashNode path = HashGet(paths_lookup[PLACES[2].id], "KL");
    printf("%s -> %d\n", path->key, ((Path) path->value)->distance);
    clock_t end = clock();
    HashTableDisplay(paths_lookup[0], print_hash_node);

    printf("%lf\n", (double) (end-start)/CLOCKS_PER_SEC);

    PrintPathSequence((Path) path->value);

//    heap_display(h);
//    EmptyHeap(h);
    return 0;
}