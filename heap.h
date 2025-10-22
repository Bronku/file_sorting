#pragma once

#include "stdbool.h"

typedef struct {
    int index;
    int buffer_id;
} heap_record;

// almost the same as typedef buffer, but the usage is different, so I think it can be repeated
typedef struct {
    heap_record* location;
    int length;
    int capacity;
    int (*compare)(const void* a, const void* b);
} heap;

heap new_heap(int size);
// free_heap is just free(h->location), so it's kinda unnecessary

int heap_pop(heap* h, heap_record* out);
int heap_push(heap* h, heap_record* in);
