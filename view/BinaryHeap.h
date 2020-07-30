/**
 * Binary Minimum Heap implementation
 */

#ifndef FURY_OF_DRACULA_BINARY_HEAP_H
#define FURY_OF_DRACULA_BINARY_HEAP_H

/**
 * Struct for Heap Representation
 */
typedef struct heap {
  int capacity;
  int size;
  struct heap_item **arr;
} * Heap;

/**
 * Struct for a heap item stored in the heap
 */
typedef struct heap_item {
  int value;
  void *data;
} * HeapItem;

/**
 * Creates a new minimum heap
 * @param initial_capacity
 * @return
 */
Heap HeapCreate(int initial_capacity);

/**
 * Pushes a new element into the heap
 * @param heap
 * @param value
 */
void HeapPush(Heap heap, HeapItem value);

/**
 * Pops the front element from a heap
 * @param heap
 * @return
 */
HeapItem HeapPop(Heap heap);

/**
 * Prints a given heap to stdout
 * @param heap
 * @param print_node Function for printing a HeapItem
 */
void HeapDisplay(Heap heap, void (*print_node)(HeapItem));

/**
 * Removes all items from the heap and frees them
 * @param heap
 */
void EmptyHeap(Heap heap);

/**
 * Frees all associated memory for a given heap
 * @param heap
 */
void HeapDestroy(Heap heap);

/**
 * Returns whether the given heap is full
 * @param heap
 * @return
 */
int HeapIsFull(Heap heap);

/**
 * Returns whether the given heap is empty
 * @param heap
 * @return
 */
int HeapIsEmpty(Heap heap);

/**
 * Creates a Heap item from the given value and data
 * @param value
 * @param data
 * @return
 */
HeapItem HeapItemCreate(int value, void *data);

#endif  // FURY_OF_DRACULA_BINARY_HEAP_H
