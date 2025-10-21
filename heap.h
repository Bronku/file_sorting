#pragma once

#include "record.h"

typedef struct {
    record* location;
    int g;
    int buffer_id;
    bool is_blank;
} heap_record;

// moves elements to maintain heap property
int heapify(heap_record* heap);
// returns the first element, and calls heapify
int heap_pop(heap_record* heap, heap_record* out);
// writes to the last elements, and calls heapify
int heap_pus(heap_record* heap, heap_record* in);
