#include "buffer.h"
#include "config.h"
#include "record.h"
#include "status_codes.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int generate_file(int N, const char* filename)
{
    srand(time(0));
    FILE* f = fopen(filename, "w");
    record rec;

    for (int i = 0; i < N; i++) {
        random_record(&rec);
        print_record(f, &rec);
        fprintf(f, "\n");
    }
    return SUCCESS;
}

int write_run_to_disk(const char* directory, int run_id, buffer* buff)
{
    char filename[256];
    sprintf(filename, "%s/%d", directory, run_id);
    FILE* tmp = fopen(filename, "w");
    write_buffer(buff, tmp);
    fclose(tmp);
    return SUCCESS;
}

int create_sorted_runs(Configuration* opts, FILE* in, buffer* buff)
{
    int runs;
    for (runs = 0; true; runs++) {
        int status = read_buffer(buff, in);
        if (status != SUCCESS && status != EOF) {
            return ERROR;
        }
        sort_buffer(buff);
        if (write_run_to_disk(opts->directory, runs, buff) != SUCCESS) {
            return ERROR;
        }
        if (status != SUCCESS) {
            break;
        }
    }
    return runs;
}

FILE** open_input_runs(const char* directory, int start_run, int count)
{
    FILE** files = malloc(count * sizeof(FILE));
    char filename[256];
    for (int i = 0; i < count; i++) {
        sprintf(filename, "%s/%d", directory, i + start_run);
        files[i] = fopen(filename, "r");
    }
    return files;
}

void close_and_delete_files(const char* directory, int start_file, int count, FILE** files)
{
    for (int i = 0; i < count; i++) {
        fclose(files[i]);
        char filename[256];
        sprintf(filename, "%s/%d", directory, start_file + i);
        remove(filename);
    }
    free(files);
}

int perform_merge_pass(Configuration* opts, buffer** buffers, int runs, int pass, int (*comapre)(const void*, const void*))
{
    int new_runs = 0;
    int run_index = 0;
    while (run_index < runs) {
        int runs_to_merge = opts->b - 1;
        if (runs - run_index < runs_to_merge) {
            runs_to_merge = runs - run_index;
        }

        FILE** input_files = open_input_runs(opts->directory, run_index, runs_to_merge);
        if (!input_files) {
            return ERROR;
        }

        FILE* output_file = open_output_file(new_runs);

        merge_runs_group();

        fclose(output_file);
        close_and_delete_files(input_files, runs_to_merge);
        new_runs++;
    }
}

int merge_all_runs(Configuration* opts, buffer* buff, int initial_runs)
{
    buffer** buffers = split_buffer(buff, opts->b);
    int runs = initial_runs;
    int merge_pass = 0;
    while (runs > 1) {
        runs = perform_merge_pass(opts, buffers, runs, merge_pass);
        merge_pass++;
        if (runs >= 0) {
            continue;
        }
        for (int i = 0; i < opts->b; i++) {
            destroy_buffer(buffers[i]);
        }
        return ERROR;
    }

    for (int i = 0; i < opts->b; i++) {
        destroy_buffer(buffers[i]);
    }
    return SUCCESS;
}

int sort_file(Configuration* opts)
{
    buffer* buff = create_buffer(opts->b * opts->n);
    FILE* in = fopen(opts->input_file, "r");
    if (!in) {
        destroy_buffer(buff);
        return ERROR;
    }

    // <stage 1> create sorted runs
    int runs = create_sorted_runs(opts, in, buff);
    fclose(in);
    if (runs < 0) {
        destroy_buffer(buff);
        return ERROR;
    }

    if (runs > 1) {
        int result = merge_all_runs(opts, buff, runs);
        destroy_buffer(buff);
        return result;
    }
    destroy_buffer(buff);
    return SUCCESS;
}

int main(int argc, char** argv)
{
    Configuration opts;
    if (argc <= 1) {
        // #todo: stdin input
        fprintf(stderr, "please specify input args\n");
        return ERROR;
    }
    int result = process_args(argc, argv, &opts);
    if (result != SUCCESS) {
        return result;
    }
    if (opts.generate_data) {
        return generate_file(opts.N, opts.output_file);
    }
    sort_file(&opts);
}
// int sort_block(tape* t, buffer* buff)
// {
//     static int counter = 0;
//     int status = read_buffer(buff, t);
//     if (status != SUCCESS && status != EOF) {
//         fprintf(stderr, "Error reading file: %d\n", status);
//         return status;
//     }

//     sort_buffer(buff);
//     printf("sorted the buffer %d, the results:\n", counter);
//     print_buffer(buff);
//     counter++;
//     return status;
// }

// int sort_file(options* opts)
// {
//     tape a = { 0, 0, opts->input };
//     buffer buff;
//     buff.capacity = opts->b * opts->n;
//     buff.location = malloc(sizeof(record) * buff.capacity);
//     int status = 0;
//     while (status == SUCCESS) {
//         status = sort_block(&a, &buff);
//     }
//     if (status != EOF) {
//         free(buff.location);
//         fclose(a.file);
//         return status;
//     }
//     free(buff.location);
//     fclose(a.file);
//     return SUCCESS;
// }
