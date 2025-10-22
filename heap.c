#include "heap.h"
#include "status_codes.h"

// #unteasted
int heap_push(heap* h, heap_record* in)
{
    if (h->length >= h->capacity) {
        return NO_SPACE;
    }

    int current_index = h->length;
    h->length++;
    heap_record* last = h->location;
    last += current_index;
    *last = *in;

    if (current_index == 0) {
        return SUCCESS;
    }

    int parent_index = (current_index - 1) / 2;
    // check if violated heap property
    heap_record* parent = h->location;
    parent += parent_index;
    heap_record* current = h->location;
    current += current_index;
    // swim up
    while (current_index != 0 && h->compare(parent, current) == 1) {
        // current, parent = parent, current
        heap_record tmp = *current;
        *current = *parent;
        *parent = tmp;
        // current_index = parent_index
        current_index = parent_index;
        // parent_index = (current_index -1 )/2
        parent_index = (current_index - 1) / 2;
        // current = loc[current_index]
        current = h->location;
        current += current_index;
        // parent = loc[parent_index]
        parent = h->location;
        parent_index += parent_index;
    }
    return SUCCESS;
}
