#include "../include/file_writer.hpp"

void FileWriter::write_buffer()
{
    for (size_t i = 0; i < current_pos_; i++) {
        stream_ << buffer_[i] << "\n";
    }
    current_pos_ = 0;
}

FileWriter::FileWriter(const std::string& filename, size_t buffer_size)
    : buffer_(buffer_size)
    , current_pos_(0)
    , total_writes_(0)
{
    std::filesystem::path file_path(filename);
    std::filesystem::path dir_path = file_path.parent_path();
    if (dir_path != "") {
        std::filesystem::create_directories(dir_path);
    }
    stream_.open(filename);
}

FileWriter::~FileWriter()
{
    write_buffer();
}

void FileWriter::write(Record record)
{
    if (current_pos_ >= buffer_.size()) {
        write_buffer();
    }
    buffer_[current_pos_] = record;
    current_pos_++;
    total_writes_++;
}

void FileWriter::write_chunk(std::span<Record> chunk)
{
    for (const auto& record : chunk) {
        write(record);
    }
}

size_t FileWriter::total_writes() const
{
    return total_writes_;
}
