#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a[5];
    int x;
} record;

static inline int g(const record* x)
{
    int out = 0;
    int x_n = 1;
    for (int i = 0; i < 5; i++) {
        out += x->a[i] * x_n;
        x_n *= x->x;
    }
    return out;
}

static inline int compare_records(const void* a, const void* b)
{
    const record* ra = (const record*)a;
    const record* rb = (const record*)b;
    int ga = g(ra);
    int gb = g(rb);
    int result = (ga > gb) - (ga < gb);
    return result;
}

static inline void print_record(FILE* stream, const record* out)
{
    fprintf(stream, "%d %d %d %d %d %d",
        out->a[0], out->a[1], out->a[2], out->a[3], out->a[4], out->x);
}

static inline int read_record(FILE* stream, record* out)
{
    record tmp;
    int result = fscanf(stream, "%d %d %d %d %d %d", &tmp.a[0], &tmp.a[1], &tmp.a[2], &tmp.a[3], &tmp.a[4], &tmp.x);
    if (result == 6) {
        *out = tmp;
        return 0;
    }
    if (result == EOF) {
        return EOF;
    }
    return -2;
}

static inline void random_record(record* out)
{
    for (int i = 0; i < 5; i++) {
        out->a[i] = rand();
    }
    out->x = rand();
}
