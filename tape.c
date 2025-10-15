#include "record.h"
#include "tape.h"

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

int read_buffer(buffer* buff, tape* tape)
{
    int status;
    record* destination = buff->location;
    buff->length = 0;
    while (buff->capacity > buff->length) {
        status = read_record(tape->file, destination);
        if (status != 0) {
            return status;
        }
        destination++;
        buff->length++;
        tape->reads++;
    }
    return 0;
}

int write_buffer(buffer* buff, tape* tape)
{
    record* current = buff->location;
    record* last = current;
    last += buff->length;
    while (current < last) {
        print_record(tape->file, current);
        fprintf(tape->file, "\n");
        current++;
        tape->reads++;
    }
    return 0;
}
