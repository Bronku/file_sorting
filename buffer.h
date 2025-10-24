#pragma once
#include "record.h"
#include <stdbool.h>

typedef struct {
    record* location;
    int length;
    int capacity;
    bool original;
} buffer;

// #todo switch to returning a pointer, and write destroy_buffer method
buffer* create_buffer(int capacity);
void destroy_buffer(buffer* buff);
buffer* split_buffer(buffer* original, int pieces);

int read_buffer(buffer* buff, FILE* in);
int write_buffer(buffer* buff, FILE* in);
int write_buffer_debug(buffer* buff, FILE* in);
void sort_buffer(buffer* buff);
