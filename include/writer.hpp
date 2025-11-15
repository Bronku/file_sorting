// #todo maybe overload << operators insted of write()
#pragma once
#include "record.hpp"
#include <ostream>
#include <vector>

class Writer {
private:
    std::vector<Record> write_buffer;
    std::ostream& output_stream;
    size_t buffer_size;
    size_t total_writes_count;
    size_t current_index;

    // returns true on success
    // doesn't really flush for performace reasons
    // could be changed to really flush to monitor the impact of amount of writes
    bool flush_buffer()
    {
        for (size_t i = 0; i < current_index; ++i) {
            if (!(output_stream << write_buffer[i] << " ")) {
                return false;
            }
        }

        current_index = 0;

        return true;
    }

public:
    explicit Writer(std::ostream& output_stream, size_t buffer_size = 8)
        : write_buffer(buffer_size)
        , output_stream(output_stream)
        , buffer_size(buffer_size)
        , total_writes_count(0)
        , current_index(0)
    {
    }

    ~Writer()
    {
        flush_buffer();
    }

    bool write(const Record& record)
    {
        write_buffer[current_index] = record;
        current_index++;
        total_writes_count++;

        if (current_index >= buffer_size) {
            return flush_buffer();
        }

        return true;
    }

    size_t total_writes() const
    {
        return total_writes_count;
    }
};
