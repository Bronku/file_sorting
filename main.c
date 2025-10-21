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
    // create buffer
    buffer buff = create_buffer(opts->b * opts->n);
    // open file
    FILE* in = fopen(opts->input_file, "r");
    // <stage 1>
    // while !eof:
    for (int i = 0; true; i++) {
        //     read buffer
        int status = read_buffer(&buff, in);
        printf("buffer i:%d, status: %d\n", i, status);
        //     sort buffer

        //     write run
        write_buffer(&buff, stdout);
        if (status != SUCCESS) {
            break;
        }
    }
    // <stage 2>
    // while tmp:
    //     merge b-1 runs
    //
    // close file
    // free buffer
    fclose(in);
    free(buff.location);
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
