#include "record.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_RECORDS 1000
#define END_PROGRAM -1
#define SUCCESS 0
#define ERROR -2

void print_usage()
{
    printf("Usage: [-f filename] [-h]\n");
    printf("Options:\n");
    printf("  -f filename\tRead records from file\n");
    printf("  -h\tShow this help message");
}

// stream is function output
int process_args(int argc, char** argv, FILE** stream)
{
    char* filename = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "f:h")) != -1) {
        switch (opt) {
        case 'f':
            filename = optarg;
            break;
        case 'h':
            print_usage();
            return END_PROGRAM;
        case '?':
            printf("invalid option, terminating\n");
            return ERROR;
        }
    }
    if (filename != NULL) {
        printf("%s\n", filename);
        *stream = fopen(filename, "r");
        if (!*stream) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
            return END_PROGRAM;
        }
    }
    return SUCCESS;
}

void process_input(FILE* input)
{
    record records[MAX_RECORDS];
    int count = 0;
    while (count < MAX_RECORDS) {
        if (read_record(input, &records[count]) != SUCCESS) {
            break;
        }
        count++;
    }

    printf("\nRead %d records\n", count);
    qsort(records, count, sizeof(record), compare_records);
    printf("\nSorted records:\n");
    for (int i = 0; i < count; i++) {
        printf("Record %d: ", i);
        print_record(stdout, &records[i]);
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    FILE* input = NULL;
    if (argc > 1) {
        int result = process_args(argc, argv, &input);
        if (result != SUCCESS) {
            return result;
        }
        if (input == NULL) {
            printf("ok\n");
        }
    }
    if (input == NULL) {
        printf("Enter records (format: a_0 a_1 a_2 a_3 a_4 x), or 'q' to quit:\n");
        input = stdin;
    }
    process_input(input);
    if (input != stdin) {
        fclose(input);
    }
}
