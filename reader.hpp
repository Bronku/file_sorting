// #todo maybe overload >> operators insted of read()
#pragma once
#include "record.hpp"
#include <istream>
#include <optional>
#include <vector>

class Reader {
private:
    std::vector<Record> read_buffer;
    std::istream& input_stream;
    size_t buffer_size;
    size_t total_reads_count;
    int last_element_idx; // last valid element in buffer
    int current_idx; // current reading position

    // returns true if at least one record was returned
    bool refill_buffer()
    {
        std::cout << "refilling buffer\n";
        last_element_idx = -1;
        current_idx = 0;

        for (size_t i = 0; i < buffer_size; i++) {
            std::cout << "buffer refill: i: " << i << '\n';
            if (!(input_stream >> read_buffer[i])) {
                std::cout << "input stream error\n";
                std::cout << read_buffer[i] << '\n';
                break;
            }
            last_element_idx = i;
        }

        std::cout << "buffer refill successfull\n";
        return last_element_idx >= 0;
    }

public:
    explicit Reader(std::istream& input_stream, size_t buffer_size = 8)
        : read_buffer(buffer_size)
        , input_stream(input_stream)
        , buffer_size(buffer_size)
        , total_reads_count(0)
        , last_element_idx(-1)
        , current_idx(0)
    {
    }

    std::optional<Record> read()
    {
        if (current_idx > last_element_idx) {
            bool read_new_items = refill_buffer();
            if (!read_new_items) {
                return std::nullopt;
            }
        }

        Record record = read_buffer[current_idx];
        current_idx++;
        total_reads_count++;

        return record;
    }

    size_t total_reads() const
    {
        return total_reads_count;
    }

    // #todo remove if not used in the end
    // bool has_more() const
    // {
    //     return current_idx <= last_element_idx || !input_stream.eof();
    // }

    // void reset()
    // {
    //     input_stream.clear();
    //     input_stream.seekg(0);
    //     last_element_idx = -1;
    //     current_idx = 0;
    //     total_reads_count = 0;
    // }
};
