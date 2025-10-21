#pragma once
#include "record.h"

typedef struct {
    record* location;
    int length;
    int capacity;
} buffer;

buffer create_buffer(int capacity);
int read_buffer(buffer* buff, FILE* in);
int write_buffer(buffer* buff, FILE* in);
int write_buffer_debug(buffer* buff, FILE* in);
void sort_buffer(buffer* buff);
