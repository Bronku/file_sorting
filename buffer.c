#include "buffer.h"
#include "record.h"
#include "status_codes.h"

buffer create_buffer(int capacity)
{
    buffer out;
    out.length = 0;
    out.capacity = capacity;
    out.location = malloc(sizeof(record) * capacity);
    return out;
}

int read_buffer(buffer* buff, FILE* in)
{
    int status;
    record* current = buff->location;
    buff->length = 0;
    for (int i = 0; i < buff->capacity; i++) {
        status = read_record(in, current);
        if (status != SUCCESS) {
            return status;
        }
        buff->length++;
        current++;
    }
    return SUCCESS;
}

int write_buffer(buffer* buff, FILE* out)
{
    int status;
    record* current = buff->location;
    for (int i = 0; i < buff->length; i++) {
        status = print_record(out, current);
        if (status < 0) {
            return status;
        }
        fprintf(out, "\n");
        current++;
    }
    return SUCCESS;
}
