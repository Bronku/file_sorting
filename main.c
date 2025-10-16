#include "record.h"
#include "tape.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_FILE "tests/1.in"
#define DEFAULT_DIR "tmp"
#define DEFAULT_NN 100000
#define DEFAULT_N 101
#define DEFAULT_B 10
#define END_PROGRAM -1
#define SUCCESS 0
#define ERROR -2

void print_usage()
{
    printf("Options:\n");
    printf("  -f [filename]\tRead/Generate records from [filename], default %s\n", DEFAULT_FILE);
    printf("  -d [dirname]\tSet the directory used for temporary files, default %s\n", DEFAULT_DIR);
    printf("  -N [count]\tSet total number of records, default %d\n", DEFAULT_NN);
    printf("  -n [count]\tSet blocking factor, default %d\n", DEFAULT_N);
    printf("  -b [count]\tSet number of buffers, default %d\n", DEFAULT_B);
    printf("  -r\t randomly generate N records\n");
    printf("  -h\tShow this help message\n");
}

typedef struct
{
    FILE* input;
    char* filename;
    char* dirname;
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
    opts->filename = DEFAULT_FILE;
    opts->dirname = DEFAULT_DIR;
    opts->generate_data = false;
    opts->manual_input = false;
    opts->N = DEFAULT_NN;
    opts->n = DEFAULT_N;
    opts->b = DEFAULT_B;
    int opt;
    while ((opt = getopt(argc, argv, "f:d:N:n:b:rhm")) != -1) {
        switch (opt) {
        case 'f':
            opts->filename = optarg;
            break;
        case 'd':
            opts->dirname = optarg;
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
            break;
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
    opts->input = fopen(opts->filename, "r+");
    if (!opts->input) {
        opts->input = fopen(opts->filename, "w+");
    }
    if (!opts->input) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", opts->filename);
        return ERROR;
    }
    struct stat st = { 0 };
    if (stat(opts->dirname, &st) == -1) {
        mkdir(opts->dirname, 0777);
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

int sort_block(tape* t, buffer* buff)
{
    static int counter = 0;
    int status = read_buffer(buff, t);
    if (status != SUCCESS && status != EOF) {
        fprintf(stderr, "Error reading file: %d\n", status);
        return status;
    }

    sort_buffer(buff);
    printf("sorted the buffer %d, the results:\n", counter);
    print_buffer(buff);
    counter++;
    return status;
}

int sort_file(options* opts)
{
    tape a = { 0, 0, opts->input };
    buffer buff;
    buff.capacity = opts->b * opts->n;
    buff.location = malloc(sizeof(record) * buff.capacity);
    int status = 0;
    while (status == SUCCESS) {
        status = sort_block(&a, &buff);
    }
    if (status != EOF) {
        free(buff.location);
        fclose(a.file);
        return status;
    }
    free(buff.location);
    fclose(a.file);
    return SUCCESS;
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
        return SUCCESS;
    }
    if (opts.manual_input) {
        write_file(&opts);
    }
    sort_file(&opts);
    fclose(opts.input);
}
