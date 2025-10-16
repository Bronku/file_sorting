#include "record.h"
#include "tape.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_NN 10000000
#define DEFAULT_N 1001
#define DEFAULT_B 10
#define END_PROGRAM -1
#define SUCCESS 0
#define ERROR -2

void print_usage()
{
    printf("Options:\n");
    printf("  -f [filename]\tRead/Generate records from [filename]\n");
    printf("  -r\t randomly generate N records, and process them\n");
    printf("  -N [count]\tSet total number of records, default %d\n", DEFAULT_NN);
    printf("  -n [count]\tSet blocking factor, default %d\n", DEFAULT_N);
    printf("  -b [count]\tSet number of buffers, default %d\n", DEFAULT_B);
    printf("  -h\tShow this help message\n");
}

typedef struct
{
    FILE* input;
    bool generate_data;
    bool manual_input;
    int N; // number of records in a file
    int b; // blocking factor
    int n; // number of buffers
} options;

// reads args into opts
int process_args(int argc, char** argv, options* opts)
{
    if (opts == NULL) {
        return ERROR;
    }
    opts->input = stdin;
    opts->generate_data = false;
    opts->manual_input = false;
    opts->N = DEFAULT_NN;
    opts->n = DEFAULT_N;
    opts->b = DEFAULT_B;
    char* filename = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "f:N:n:b:rhm")) != -1) {
        switch (opt) {
        case 'f':
            filename = optarg;
            break;
        case 'r':
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
        case 'm':
            opts->manual_input = true;
        case 'h':
            print_usage();
            return END_PROGRAM;
        case '?':
            fprintf(stderr, "invalid option, terminating\n");
            return ERROR;
        }
    }
    if (opts->manual_input == true && opts->generate_data == true) {
        fprintf(stderr, "Incompatible arguments: mr, aborting\n");
        return ERROR;
    }
    if (filename == NULL) {
        fprintf(stderr, "No file specified, aborting\n");
        return ERROR;
    }
    opts->input = fopen(filename, "r+");
    if (!opts->input) {
        opts->input = fopen(filename, "w+");
    }
    if (!opts->input) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return ERROR;
    }
    return SUCCESS;
}

void generate_file(options* opts)
{
    srand(time(0));
    record rec;
    for (int i = 0; i < opts->N; i++) {
        random_record(&rec);
        print_record(opts->input, &rec);
        fprintf(opts->input, "\n");
    }
    fseek(opts->input, 0, SEEK_SET);
}

void write_file(options* opts)
{
}

void sort_file(options* opts)
{
    tape a = { 0, 0, opts->input };
    buffer buff;
    buff.capacity = 100;
    buff.location = malloc(sizeof(record) * buff.capacity);
    int status = read_buffer(&buff, &a);
    if (status != SUCCESS && status != EOF) {
        printf("Error reading file: %d\n", status);
    }
    fclose(a.file);
    a.file = stdout;
    sort_buffer(&buff);
    free(buff.location);
}

int main(int argc, char** argv)
{
    options opts;
    if (argc <= 1) {
        fprintf(stderr, "please specify input args\n");
        return ERROR;
    }
    int result = process_args(argc, argv, &opts);
    if (result != SUCCESS) {
        return result;
    }
    if (opts.generate_data) {
        generate_file(&opts);
    }
    if (opts.manual_input) {
        write_file(&opts);
    }
    sort_file(&opts);
    fclose(opts.input);
}
