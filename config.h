#pragma once
#include "status_codes.h"
#include <stdbool.h>

#define DEFAULT_IN "1.in"
#define DEFAULT_OUT "1.in"
#define DEFAULT_DIR "tmp"
#define DEFAULT_NN 100000
#define DEFAULT_N 101
#define DEFAULT_B 10

typedef struct
{
    const char* input_file;
    const char* output_file;
    const char* directory;
    bool generate_data;
    int N; // number of records in a file
    int b; // blocking factor
    int n; // number of buffers
} Configuration;

int process_args(int argc, char** argv, Configuration* opts);
