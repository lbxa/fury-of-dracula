//
// Created by eric on 16/7/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "binary_heap.h"


Heap *heap_create(int initial_capacity) {
    Heap *heap = malloc(sizeof(struct heap));
    heap->capacity = initial_capacity;
    heap->size = 0;
    heap->arr = malloc(sizeof(int) * heap->capacity);
    assert(heap->arr != NULL);
    return heap;
}

void heap_display(Heap *heap) {
    int i;
    for (i = 1; i <= heap->size; ++i) {
        printf("|%d|", heap->arr[i]);
    }
    printf("\n");
}

void swap(int *arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int less(const int *arr, int i, int j) {
    return arr[i] < arr[j];
}

void heapify_up(int *arr, int i) {
    while (i > 1 && less(arr, i/2, i)) {
        swap(arr, i, i/2);
        i = i / 2;
    }
}

void heapify_down(int *arr, int i, int n) {
    while (i * 2 <= n) {
        int j = i * 2;
        if (j < n && arr[j] > arr[j+1]) {
            j++;
        }
        if (!less(arr, i, j)) break;
        swap(arr, i, j);
        i = j;
    }
}

void heap_push(Heap *heap, int value) {
    if (is_heap_full(heap)) {
        heap->capacity *= 2;
        heap->arr = realloc(heap->arr, sizeof(int) * heap->capacity);
        assert(heap->arr != NULL);
    }
    heap->arr[++heap->size] = value;
}

int heap_pop(Heap *heap) {
    int top = heap->arr[1];
    heap->arr[1] = heap->arr[heap->size];
    heap->size--;
    heapify_down(heap->arr, 1, heap->size);
    return top;
}

void heap_destroy(Heap *heap) {
    free(heap->arr);
    free(heap);
}

int is_heap_full(Heap *heap) {
    return heap->size >= heap->capacity;
}

int is_heap_empty(Heap *heap) {
    return heap->size == 0;
}


int empty_heap(Heap *heap) {
    while (heap->size) {
        heap_pop(heap);
    }
}