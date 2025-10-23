// doesn't ceck for user erros, so no veryfying that the void* supplied are valid
// unsafe for general use, but just fnuctional enough for the purposes of this project
// does not maintain ownership of its elements, they are managed by the caller
#pragma once

#include "stdbool.h"

typedef struct {
    void** data;
    int length;
    int capacity;
    int (*compare)(const void* a, const void* b);
} heap;

heap* create_heap(int size, int (*comparison)(const void* a, const void* b));
void destroy_heap(heap* h);

int heap_pop(heap* h, void** out);
int heap_push(heap* h, void* in);
