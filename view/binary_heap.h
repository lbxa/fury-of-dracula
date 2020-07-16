/**
 * Binary Heap implementation for use as priority queue
 * for implementing fast djikstra search algorithm
 */

#ifndef FURY_OF_DRACULA_BINARY_HEAP_H
#define FURY_OF_DRACULA_BINARY_HEAP_H

typedef struct heap {
    int capacity;
    int size;
    struct heap_item  **arr;
} *Heap;

typedef struct heap_item {
    int value;
    char *key;
} *HeapItem;

Heap heap_create(int initial_capacity);

void heap_push(Heap heap, HeapItem value);

HeapItem heap_pop(Heap heap);

void heap_display(Heap heap);

void empty_heap(Heap heap);

void heap_destroy(Heap heap);

int is_heap_full(Heap heap);

int is_heap_empty(Heap heap);

HeapItem create_heap_item(int value, const char *key);


#endif //FURY_OF_DRACULA_BINARY_HEAP_H
