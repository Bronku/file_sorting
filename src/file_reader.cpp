#include "../include/file_reader.hpp"

void FileReader::refill_buffer()
{
    current_pos_ = 0;
    valid_items_ = 0;

    for (size_t i = 0; i < buffer_.size(); ++i) {
        Record record;
        if (!(stream_ >> record)) {
            break;
        }
        buffer_[i] = record;
        valid_items_ = i + 1;
    }
}

FileReader::FileReader(const std::string& filename, size_t buffer_size)
    : stream_(filename)
    , buffer_(buffer_size)
    , current_pos_(0)
    , valid_items_(0)
    , total_reads_(0)
{
    refill_buffer();
}

std::optional<Record> FileReader::read()
{
    if (current_pos_ >= valid_items_) {
        refill_buffer();
    }

    if (valid_items_ == 0) {
        return std::nullopt;
    }

    Record record = buffer_[current_pos_];
    current_pos_++;
    total_reads_++;

    return record;
}

size_t FileReader::read_chunk(std::span<Record>& chunk)
{
    size_t count = 0;
    for (size_t i = 0; i < chunk.size(); i++) {
        auto record = read();
        if (!record.has_value()) {
            break;
        }
        chunk[count] = record.value();
        count++;
    }
    return count;
}

size_t FileReader::total_reads() const
{
    return total_reads_;
}
