//
// Created by eric on 16/7/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "binary_heap.h"

Heap heap_create(int initial_capacity) {
    Heap heap = malloc(sizeof(struct heap));
    heap->capacity = initial_capacity;
    heap->size = 0;
    heap->arr = malloc(sizeof(HeapItem) * heap->capacity);
    assert(heap->arr != NULL);
    return heap;
}

void heap_display(Heap heap, void (*print_node)(HeapItem)) {
    for (int i = 1; i <= heap->size; ++i) {
        print_node(heap->arr[i]);
    }
    printf("\n");
}

void swap(HeapItem *arr, int i, int j) {
    HeapItem temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int less(const HeapItem *arr, int i, int j) {
    return arr[i]->value < arr[j]->value;
}

void heapify_up(HeapItem *arr, int i) {
    while (i > 1 && less(arr, i/2, i)) {
        swap(arr, i, i/2);
        i = i / 2;
    }
}

void heapify_down(HeapItem *arr, int i, int n) {
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

void heap_push(Heap heap, HeapItem value) {
    if (is_heap_full(heap)) {
        heap->capacity *= 2;
        heap->arr = realloc(heap->arr, sizeof(struct heap_item) * heap->capacity);
        assert(heap->arr != NULL);
    }
    heap->arr[++heap->size] = value;
    heapify_up(heap->arr, heap->size);
}

HeapItem heap_pop(Heap heap) {
    HeapItem top = heap->arr[1];
    heap->arr[1] = heap->arr[heap->size];
    heap->size--;
    heapify_down(heap->arr, 1, heap->size);
    return top;
}

HeapItem create_heap_item(int value, void *data) {
    HeapItem heap_item = malloc(sizeof(struct heap_item));
    heap_item->value = value;
    heap_item->data = data;
    return heap_item;
}

void heap_destroy(Heap heap) {
    for (int i = 1; i < heap->size; i++) {
        free(heap->arr[i]);
    }
    free(heap->arr);
    free(heap);
}

int is_heap_full(Heap heap) {
    return heap->size >= heap->capacity;
}

int is_heap_empty(Heap heap) {
    return heap->size == 0;
}

void empty_heap(Heap heap) {
    while (heap->size) {
        heap_pop(heap);
    }
}