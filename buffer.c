#include "buffer.h"
#include "record.h"
#include "status_codes.h"
#include <stdbool.h>
#include <string.h>

buffer* create_buffer(int capacity)
{
    buffer* out = malloc(sizeof(buffer));
    out->length = 0;
    out->capacity = capacity;
    out->location = malloc(sizeof(record) * capacity);
    out->original = true;
    return out;
}

void destroy_buffer(buffer* buff)
{
    if (buff->original) {
        free(buff->location);
    }
    free(buff);
}

// #todo
buffer* split_buffer(buffer* original, int pieces)
{
    return NULL;
}

int read_buffer(buffer* buff, FILE* in)
{
    int status;
    record* current = buff->location;
    buff->length = 0;
    for (int i = 0; i < buff->capacity; i++) {
        status = read_record(in, current);
        if (status != SUCCESS) {
            return status;
        }
        buff->length++;
        current++;
    }
    return SUCCESS;
}

int write_buffer(buffer* buff, FILE* out)
{
    int status;
    record* current = buff->location;
    for (int i = 0; i < buff->length; i++) {
        status = print_record(out, current);
        if (status < 0) {
            return status;
        }
        fprintf(out, "\n");
        current++;
    }
    return SUCCESS;
}

int write_buffer_debug(buffer* buff, FILE* out)
{
    int status;
    record* current = buff->location;
    for (int i = 0; i < buff->length; i++) {
        fprintf(out, "%d\t", g(current));
        status = print_record(out, current);
        if (status < 0) {
            return status;
        }
        fprintf(out, "\n");
        current++;
    }
    return SUCCESS;
}

int partition(record* arr, int start, int end)
{
    int i = start;
    record* pivot = arr;
    pivot += end;
    for (int j = start; j < end; j++) {
        record* current = arr;
        current += j;
        if (compare_records(current, pivot) > 0) {
            continue;
        }
        record* i_rec = arr;
        i_rec += i;
        record tmp = *i_rec;
        *i_rec = *current;
        *current = tmp;
        i++;
    }
    record* i_rec = arr;
    i_rec += i;
    record tmp = *i_rec;
    *i_rec = *pivot;
    *pivot = tmp;
    return i;
}

void quicksort(record* arr, int start, int end, int depth)
{
    if (start >= end || start < 0) {
        return;
    }

    int p = partition(arr, start, end);
    quicksort(arr, start, p - 1, depth + 1);
    quicksort(arr, p + 1, end, depth + 1);
}

void sort_buffer(buffer* buff)
{
    quicksort(buff->location, 0, buff->length - 1, 0);
}
