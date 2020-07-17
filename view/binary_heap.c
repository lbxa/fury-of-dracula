//
// Created by eric on 16/7/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "binary_heap.h"

Heap HeapCreate(int initial_capacity) {
    Heap heap = malloc(sizeof(struct heap));
    heap->capacity = initial_capacity;
    heap->size = 0;
    heap->arr = malloc(sizeof(HeapItem) * heap->capacity);
    assert(heap->arr != NULL);
    return heap;
}

void HeapDisplay(Heap heap, void (*print_node)(HeapItem)) {
    for (int i = 1; i <= heap->size; ++i) {
        print_node(heap->arr[i]);
    }
    printf("\n");
}

/**
 * Swaps 2 given values at indexes i and j with each other
 * @param arr
 * @param i
 * @param j
 */
void swap(HeapItem *arr, int i, int j) {
    HeapItem temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

/**
 * Returns the lower of the items at indexes i and j
 * @param arr
 * @param i
 * @param j
 * @return
 */
int less(const HeapItem *arr, int i, int j) {
    return arr[i]->value < arr[j]->value;
}

/**
 * Performs a heapify up operation for insertion handling
 * @param arr
 * @param i
 */
void HeapifyUp(HeapItem *arr, int i) {
    while (i > 1 && less(arr, i/2, i)) {
        swap(arr, i, i/2);
        i = i / 2;
    }
}

/**
 * Performs a heapify down operation for deletion handling
 * @param arr
 * @param i
 * @param n
 */
void HeapifyDown(HeapItem *arr, int i, int n) {
    while (i * 2 <= n) {
        int j = i * 2;
        if (j < n && less(arr, j, j + 1)) {
            j++;
        }
        if (!less(arr, i, j)) break;
        swap(arr, i, j);
        i = j;
    }
}

void HeapPush(Heap heap, HeapItem value) {
    if (IsHeapFull(heap)) {
        heap->capacity *= 2;
        heap->arr = realloc(heap->arr, sizeof(struct heap_item) * heap->capacity);
        assert(heap->arr != NULL);
    }
    heap->arr[++heap->size] = value;
    HeapifyUp(heap->arr, heap->size);
}

HeapItem HeapPop(Heap heap) {
    HeapItem top = heap->arr[1];
    heap->arr[1] = heap->arr[heap->size];
    heap->size--;
    HeapifyDown(heap->arr, 1, heap->size);
    return top;
}

HeapItem CreateHeapItem(int value, void *data) {
    HeapItem heap_item = malloc(sizeof(struct heap_item));
    heap_item->value = value;
    heap_item->data = data;
    return heap_item;
}

void HeapDestroy(Heap heap) {
    for (int i = 1; i < heap->size; i++) {
        free(heap->arr[i]);
    }
    free(heap->arr);
    free(heap);
}

int IsHeapFull(Heap heap) {
    return heap->size >= heap->capacity;
}

int IsHeapEmpty(Heap heap) {
    return heap->size == 0;
}

void EmptyHeap(Heap heap) {
    while (heap->size) {
        HeapPop(heap);
    }
}