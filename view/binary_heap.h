/**
 * Binary Heap implementation for use as priority queue
 * for implementing fast djikstra search algorithm
 */

#ifndef FURY_OF_DRACULA_BINARY_HEAP_H
#define FURY_OF_DRACULA_BINARY_HEAP_H

typedef struct heap {
    int capacity;
    int size;
    int *arr;
} Heap;

Heap *heap_create(int initial_capacity);

void heap_push(Heap *heap, int value);

int heap_pop(Heap *heap);

void heap_display(Heap *heap);

int empty_heap(Heap *heap);

void heap_destroy(Heap *heap);

int is_heap_full(Heap *heap);

int is_heap_empty(Heap *heap);


#endif //FURY_OF_DRACULA_BINARY_HEAP_H
