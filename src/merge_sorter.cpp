#include "../include/merge_sorter.hpp"

std::span<Record> MergeSorter::get_chunk_span(size_t chunk_index)
{
    size_t chunk_size = buffer_.size() / buffer_rows_;
    size_t start = chunk_index * chunk_size;
    return std::span<Record>(buffer_).subspan(start, chunk_size);
}

size_t MergeSorter::create_initial_runs(const std::string& input_file, const std::string& next_dir)
{
    FileReader reader(input_file);
    std::span<Record> chunk(buffer_);
    size_t run_count = 0;

    for (;; run_count++) {
        size_t records_read = reader.read_chunk(chunk);
        if (records_read == 0) {
            break;
        }

        std::sort(chunk.begin(), chunk.begin() + records_read);

        std::string run_file = next_dir + std::to_string(run_count) + ".dat";
        FileWriter writer(run_file);
        auto output = std::span<Record>(chunk.data(), records_read);
        writer.write_chunk(output);

        disk_writes_ += writer.total_writes();
    }
    disk_reads_ += reader.total_reads();

    return run_count;
}

void MergeSorter::perform_k_way_merge(std::vector<std::unique_ptr<FileReader>>& readers,
    FileWriter& writer)
{
    using HeapElement = std::pair<Record, size_t>;
    auto heap_compare = [](const HeapElement& a, const HeapElement& b) {
        return !(a.first < b.first);
    };
    std::priority_queue<HeapElement, std::vector<HeapElement>, decltype(heap_compare)>
        min_heap(heap_compare);

    std::vector<std::span<Record>> buffers;
    std::vector<size_t> positions;
    std::vector<size_t> chunk_sizes;

    buffers.reserve(readers.size());
    positions.reserve(readers.size());
    chunk_sizes.reserve(readers.size());

    for (size_t i = 0; i < readers.size(); ++i) {
        buffers.push_back(get_chunk_span(i));
        chunk_sizes.push_back(readers[i]->read_chunk(buffers.back()));
        positions.push_back(0);

        if (chunk_sizes[i] > 0) {
            min_heap.emplace(buffers[i][0], i);
            positions[i] = 1;
        }
    }

    auto output_buffer = get_chunk_span(buffer_rows_ - 1);
    size_t output_pos = 0;

    while (!min_heap.empty()) {
        auto [record, source_idx] = min_heap.top();
        min_heap.pop();

        output_buffer[output_pos++] = record;

        if (output_pos >= output_buffer.size()) {
            writer.write_chunk(output_buffer);
            output_pos = 0;
        }

        if (positions[source_idx] >= chunk_sizes[source_idx]) {
            chunk_sizes[source_idx] = readers[source_idx]->read_chunk(buffers[source_idx]);
            positions[source_idx] = 0;
            if (chunk_sizes[source_idx] == 0)
                continue;
        }

        if (positions[source_idx] < chunk_sizes[source_idx]) {
            min_heap.emplace(buffers[source_idx][positions[source_idx]], source_idx);
            positions[source_idx]++;
        }
    }

    if (output_pos > 0) {
        writer.write_chunk(std::span<Record>(output_buffer.data(), output_pos));
    }
}

size_t MergeSorter::merge_pass(const std::string& current_dir, const std::string& next_dir, size_t run_count)
{
    const size_t merge_factor = buffer_rows_ - 1;
    const size_t chunk_size = buffer_.size() / buffer_rows_;
    size_t new_run_count = 0;

    for (size_t i = 0; i < run_count; i += merge_factor) {
        size_t end = std::min(i + merge_factor, run_count);

        std::vector<std::unique_ptr<FileReader>> readers;
        for (size_t j = i; j < end; ++j) {
            std::string input_filename = current_dir + std::to_string(j) + ".dat";
            readers.push_back(std::make_unique<FileReader>(input_filename, chunk_size));
        }

        std::string output_filename = next_dir + std::to_string(new_run_count) + ".dat";
        FileWriter writer(output_filename, chunk_size);

        perform_k_way_merge(readers, writer);

        for (const auto& reader : readers) {
            disk_reads_ += reader->total_reads();
        }
        disk_writes_ += writer.total_writes();

        new_run_count++;
    }

    return new_run_count;
}

MergeSorter::MergeSorter(size_t buffer_rows, size_t buffer_cols, const std::string& tmp_dir)
    : buffer_(buffer_rows * buffer_cols)
    , tmp_dir_(tmp_dir)
    , buffer_rows_(buffer_rows)
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
    std::string current_dir = tmp_dir_ + "/pass0/run_";
    std::string next_dir = tmp_dir_ + "/pass1/run_";
    size_t run_count = 0;

    run_count = create_initial_runs(input_file, current_dir);
    phases_++;
    std::cout << "created inital runs, disk reads: " << disk_reads_ << ", disk writes: " << disk_writes_ << '\n';

    while (run_count > 1) {
        run_count = merge_pass(current_dir, next_dir, run_count);
        phases_++;
        current_dir = next_dir;
        next_dir = tmp_dir_ + "/pass" + std::to_string(phases_) + "/run_";
    }

    std::cout << output_file << '\n';
    return true;
}
