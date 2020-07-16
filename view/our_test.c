//
// Created by eric on 16/7/20.
//

#include "binary_heap.h"
#include <stdio.h>

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
//    heap_display(h);
//    empty_heap(h);
    return 0;
}