#pragma once
#include "file_reader.hpp"
#include "file_writer.hpp"
#include "record.hpp"
#include <vector>

class MergeSorter {
private:
    std::vector<Record> buffer_;
    const std::string& tmp_dir_;
    size_t buffer_rows_ = 0;
    size_t disk_reads_ = 0;
    size_t disk_writes_ = 0;
    size_t phases_ = 0;

    std::span<Record> get_chunk_span(size_t chunk_index);
    size_t create_initial_runs(const std::string& input_file, const std::string& next_dir);
    size_t merge_pass(const std::string& current_dir, const std::string& next_dir, size_t run_count);
    void perform_k_way_merge(std::vector<std::unique_ptr<FileReader>>& readers,
        FileWriter& writer);

public:
    MergeSorter(size_t buffer_rows, size_t buffer_cols, const std::string& tmp_dir);
    void sort_file(const std::string& input_file, const std::string& output_file);

    size_t disk_reads();
    size_t disk_writes();
    size_t phases();
};
