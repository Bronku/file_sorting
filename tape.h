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
void sort_buffer(buffer* buff);

// reads [capacity] records
int read_buffer(buffer* buff, tape* tape);
// writes everything
int write_buffer(buffer* buff, tape* tape);
// for debugging
void print_buffer(buffer* buff);

void clear_tape(tape* tape);
