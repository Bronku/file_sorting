#pragma once
#include "record.hpp"
#include <fstream>
#include <optional>
#include <span>

class FileReader {
private:
    std::ifstream stream_;
    std::vector<Record> buffer_;
    size_t current_pos_;
    size_t valid_items_;
    size_t total_reads_;

    void refill_buffer();

public:
    FileReader(const std::string& filename, size_t buffer_size = 8);
    std::optional<Record> read();
    size_t read_chunk(std::span<Record>& chunk);
    size_t total_reads() const;
};
