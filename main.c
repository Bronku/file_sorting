#include "buffer.h"
#include "config.h"
#include "record.h"
#include "status_codes.h"
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

int sort_file(Configuration* opts)
{
    buffer* buff = create_buffer(opts->b * opts->n);
    FILE* in = fopen(opts->input_file, "r");
    int runs;
    // <stage 1>
    for (runs = 0; true; runs++) {
        //     read buffer
        int status = read_buffer(buff, in);
        if (status != SUCCESS && status != EOF) {
            fclose(in);
            destroy_buffer(buff);
        }
        //     sort buffer
        sort_buffer(buff);
        //     write run
        char* filename = malloc(256);
        sprintf(filename, "%s/%d", opts->directory, runs);
        FILE* tmp = fopen(filename, "w");
        write_buffer(buff, tmp);
        // write_buffer_debug(buff, stdout);
        fclose(tmp);
        free(filename);
        if (status != SUCCESS) {
            break;
        }
    }
    printf("created runs: %d\n", runs);

    // <stage 2>
    while (runs > 1) {

        // split buffers
        // read buffers
        for (int i = 0; i < opts->b - 1; i++) {
        }

        // b-1 runs are turned into one, so the total is reduced by b-2
        runs -= (opts->b - 2);
    }
    fclose(in);
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
