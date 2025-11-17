#pragma once
#include "record.hpp"
#include <fstream>
#include <span>

class FileWriter {
private:
    std::ofstream stream_;
    std::vector<Record> buffer_;
    size_t current_pos_;
    size_t total_writes_;

    void write_buffer();

public:
    FileWriter(const std::string& filename, size_t buffer_size = 8);
    ~FileWriter();

    void write(const Record& record);
    void write_chunk(std::span<const Record>& chunk);

    size_t total_writes() const;
};
