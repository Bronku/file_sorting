#include "../include/merge_sorter.hpp"

size_t MergeSorter::create_initial_runs(const std::string& input_file)
{
    FileReader reader(input_file);
    std::span<Record> chunk(buffer_);
    size_t run_count = 0;

    for (run_count = 0;; run_count++) {
        size_t records_read = reader.read_chunk(chunk);
        if (records_read == 0) {
            break;
        }

        std::sort(chunk.begin(), chunk.begin() + records_read);

        std::string run_file = tmp_dir_ + "pass0/run_" + std::to_string(run_count) + ".dat";
        FileWriter writer(run_file);
        auto output = std::span<const Record>(chunk.data(), records_read);
        writer.write_chunk(output);

        disk_reads_ += reader.total_reads() - disk_reads_;
        disk_writes_ += writer.total_writes() - disk_writes_;
    }

    return run_count;
}
MergeSorter::MergeSorter(size_t buffer_rows, size_t buffer_cols, const std::string& tmp_dir)
    : buffer_(buffer_rows * buffer_cols)
    , tmp_dir_(tmp_dir)
    , buffer_rows(buffer_rows)
    , disk_reads_(0)
    , disk_writes_(0)
    , phases_(0)
{
    if (buffer_rows < 3) {
        throw std::invalid_argument("Buffer must have at least 3 rows for merging");
    }
}

bool MergeSorter::sort_file(const std::string& input_file, const std::string& output_file)
{
    create_initial_runs(input_file);
    std::cout << output_file << '\n';
    return true;
}
