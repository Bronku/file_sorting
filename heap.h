#pragma once

#include "record.h"
#include "stdbool.h"

typedef struct {
    record* location;
    int g;
    int buffer_id;
    bool is_blank;
} heap_record;

// almost the same as typedef buffer, but the usage is different, so I think it can be repeated
typedef struct {
    heap_record* location;
    int length;
    int capacity;
} heap;

heap new_heap(int size);
// free_heap is just free(h->location), so it's kinda unnecessary

int heap_pop(heap* h, heap_record* out);
int heap_push(heap* h, heap_record* in);
