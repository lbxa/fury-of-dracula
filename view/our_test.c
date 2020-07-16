//
// Created by eric on 16/7/20.
//

#include "binary_heap.h"
#include <stdio.h>

int main(void) {

    Heap *h = heap_create(32);
    heap_push(h,1);
//    heap_push(h,5);
//    heap_push(h,3);
//    heap_push(h,7);
//    heap_push(h,9);
//    heap_push(h,8);
    heap_display(h);
    int v = heap_pop(h);
    printf("%d\n", v);
//    heap_display(h);
//    empty_heap(h);
    return 0;
}