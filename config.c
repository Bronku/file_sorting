#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int process_args(int argc, char** argv, Configuration* opts)
{
    opts->input_file = DEFAULT_IN;
    opts->output_file = DEFAULT_OUT;
    opts->directory = DEFAULT_DIR;
    opts->generate_data = false;
    opts->N = DEFAULT_NN;
    opts->n = DEFAULT_N;
    opts->b = DEFAULT_B;
    int opt;
    bool input_set = false;
    while ((opt = getopt(argc, argv, "i:o:d:gN:n:b:")) != -1) {
        switch (opt) {
        case 'i':
            opts->input_file = optarg;
            input_set = true;
            break;
        case 'o':
            opts->output_file = optarg;
            break;
        case 'd':
            opts->directory = optarg;
            break;
        case 'g':
            opts->generate_data = true;
            break;
        case 'N':
            opts->N = atoi(optarg);
            break;
        case 'n':
            opts->n = atoi(optarg);
            break;
        case 'b':
            opts->b = atoi(optarg);
            break;
        case '?':
            printf("wrong argument\n");
            return ERROR;
        }
    }
    if (opts->generate_data && input_set) {
        printf("incompatible arguments\n");
        return ERROR;
    }
    return SUCCESS;
}
