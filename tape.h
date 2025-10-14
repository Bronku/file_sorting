#pragma once
#include <stdio.h>

typedef struct {
    int reads;
    int writes;
    FILE* file;
} tape;

typedef struct {
    void* location;
    int length;
    int capacity;
} buffer;

// sorts the entire buffer
void quicksort(buffer* buff);

// reads [capacity] records
void read_buffer(buffer* buff, tape* tape);
// writes everything
void write_buffer(buffer* buff, tape* tape);

void clear_tape(tape* tape);
