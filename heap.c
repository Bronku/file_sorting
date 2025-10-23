#include "heap.h"
#include "status_codes.h"
#include <stdlib.h>

heap* create_heap(int size, int (*comparison)(const void* a, const void* b))
{
    heap* out = malloc(sizeof(heap));
    out->data = malloc(sizeof(void*) * size);
    out->length = 0;
    out->capacity = size;
    out->compare = comparison;
    return out;
}

void destroy_heap(heap* h)
{
    free(h->data);
    free(h);
}

void swap(void** a, void** b)
{
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}

int heap_push(heap* h, void* in)
{
    int current = h->length;
    void** d = h->data;
    h->length++;
    d[current] = in;
    if (current == 0) {
        return SUCCESS;
    }
    int parent = (current - 1) / 2;
    while (h->compare(d[current], d[parent]) == -1) {
        swap(&d[current], &d[parent]);
        current = parent;
        if (current == 0) {
            break;
        }
        parent = (current - 1) / 2;
    }
    return SUCCESS;
}

int heap_pop(heap* h, void** out)
{
    void** d = h->data;
    *out = d[0];
    h->length--;
    if (h->length == 0) {
        return SUCCESS;
    }
    swap(&d[0], &d[h->length]);
    int current = 0;
    int left = current * 2 + 1;
    int right = current * 2 + 2;
    while (true) {
        int smallest = current;
        if (left < h->length && h->compare(d[smallest], d[left]) == 1) {
            smallest = left;
        }
        if (right < h->length && h->compare(d[smallest], d[right]) == 1) {
            smallest = right;
        }
        if (smallest == current) {
            break;
        }
        swap(&d[smallest], &d[current]);
        current = smallest;
        left = current * 2 + 1;
        right = current * 2 + 2;
    }
    return SUCCESS;
}
