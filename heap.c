#include "heap.h"
#include "status_codes.h"

int heap_push(heap* h, heap_record* in)
{
    if (h->length >= h->capacity) {
        return NO_SPACE;
    }

    int index = h->length;
    h->length++;
    heap_record* last = h->location;
    last += index;
    *last = *in;

    if (index == 0) {
        return SUCCESS;
    }

    // check if violated heap property
    heap_record* parent = h->location;
    parent += ((index - 1) / 2);
    while (parent->g > last->g) {
        // just swap them, and check again?
    }
    // swim up
    return SUCCESS;
}
