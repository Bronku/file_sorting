#pragma once
#include "buffer.hpp"
#include "config.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include <exception>
#include <fstream>
#include <queue>
#include <stdexcept>
#include <string>

// returns amount of records read
size_t read_chunk(Reader& reader, SubBuffer& buff)
{
    for (size_t i = 0; i < buff.size(); i++) {
        std::optional<Record> in = reader.read();
        if (!in.has_value()) {
            return i;
        }
        buff[i] = in.value();
    }
    return buff.size();
}

void write_to_file(const std::string& filename, SubBuffer& buff, size_t n)
{
    std::filesystem::path dirPath = std::filesystem::path(filename).parent_path();
    std::filesystem::create_directories(dirPath);

    std::ofstream out_stream(filename);
    Writer output_writer(out_stream);
    for (size_t j = 0; j < n; j++) {
        output_writer.write(buff[j]);
    }
}

size_t create_initial_runs(const std::string& input_filename, const std::string& directory, Buffer& main_buffer)
{
    auto buff = main_buffer.divide(1)[0];

    std::ifstream in_stream(input_filename);
    Reader input_reader(in_stream);
    size_t run_index;

    for (run_index = 0;; run_index++) {
        size_t size = read_chunk(input_reader, buff);
        if (size == 0) {
            break;
        }
        std::sort(buff.begin(), buff.begin() + size);
        write_to_file(directory + "/" + std::to_string(run_index) + ".run", buff, size);
    }

    return run_index;
}

void write_block(Writer& w, SubBuffer& buff, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        w.write(buff[i]);
    }
}

void k_way_merge_and_write(std::vector<Reader>& input_readers, Writer& output_writer, std::vector<SubBuffer>& buffers)
{
    // for debug purposes
    std::cout << "buffers size: " << buffers.size() << " input_readers: " << input_readers.size() << '\n';
    for (size_t i = 0; i < input_readers.size(); i++) {
        std::cout << "reader " << i << ": " << input_readers[i].read().value() << '\n';
    }

    struct HeapElement {
        Record record;
        size_t source_index;
        // priority queue is a max heap by default, so comparison is backwards
        bool operator<(const HeapElement& other) const
        {
            return !(record < other.record);
        }
    };
    using MinHeap = std::priority_queue<HeapElement>;
    MinHeap pq;

    struct BufferState {
        size_t items_total = 0;
        size_t current_idx = 0;
    };

    std::vector<BufferState> buffer_states(input_readers.size());

    // initial fill
    for (size_t i = 0; i < input_readers.size(); i++) {
        SubBuffer& input_buffer = buffers[i];
        buffer_states[i].items_total = read_chunk(input_readers[i], input_buffer);
        buffer_states[i].current_idx = 0;

        if (buffer_states[i].items_total == 0) {
            continue;
        }

        pq.push({ input_buffer[0], i });
        buffer_states[i].current_idx++;
    }

    size_t output_index = 0;
    SubBuffer& output_buffer = buffers[buffers.size() - 1];

    while (!pq.empty()) {
        HeapElement next = pq.top();
        pq.pop();

        output_buffer[output_index] = next.record;
        output_index++;

        if (output_index == output_buffer.size()) {
            write_block(output_writer, output_buffer, output_index);
            output_index = 0;
        }

        size_t source_index = next.source_index;

        if (buffer_states[source_index].current_idx >= buffer_states[source_index].items_total) {
            buffer_states[source_index].items_total = read_chunk(input_readers[source_index], buffers[source_index]);
            buffer_states[source_index].current_idx = 0;
            if (buffer_states[source_index].items_total == 0) {
                continue;
            }
        }

        pq.push({ buffers[source_index][buffer_states[source_index].current_idx], source_index });
        buffer_states[source_index].current_idx++;
    }
}

// I think more or less works correctly now
// creates input readers, and output writer for a merge to be performed by another function
// if not every file can be processed at once, splits them
size_t run_merge_pass(std::vector<SubBuffer>& buffers, std::string current_dir, std::string next_dir, size_t file_count)
{
    const size_t K = buffers.size() - 1; // Merge factor (e.g., 3)

    size_t new_runs = 0;
    for (size_t i = 0; i < file_count; i += K) {
        // std::cout << "pass 0 run " << i << '\n';
        // create input readers, to turn into a function i would have to some complicated ownership semantics, and i don't want to bother with that
        std::vector<std::ifstream> in_streams;
        std::vector<Reader> input_readers;
        in_streams.reserve(K);
        input_readers.reserve(K);
        for (size_t j = 0; j < K; j++) {
            std::string filename = current_dir + "/" + std::to_string(i + j) + ".run";
            in_streams.emplace_back(std::ifstream(filename));
            input_readers.emplace_back(Reader(in_streams[j]));
        }

        // create the output writer -||-
        std::string filename = next_dir + "/" + std::to_string(new_runs) + ".run";
        std::filesystem::path dirPath = std::filesystem::path(filename).parent_path();
        std::filesystem::create_directories(dirPath);
        std::ofstream out_stream(filename);
        Writer output_writer(out_stream);

        k_way_merge_and_write(input_readers, output_writer, buffers);
        new_runs++;
    }

    return new_runs;
}

int sort_file(const Configuration& opts)
{
    Buffer main_buffer(opts.n * opts.b);
    size_t file_count;

    file_count = create_initial_runs(opts.input_file, opts.directory + "/pass0", main_buffer);

    auto buffers = main_buffer.divide(opts.n);
    for (int i = 0;; i++) {
        std::string current_dir = opts.directory + "/pass" + std::to_string(i);
        std::string next_dir = opts.directory + "/pass" + std::to_string(i + 1);
        file_count = run_merge_pass(buffers, current_dir, next_dir, file_count);
        if (file_count == 1) {
            // #todo move the resulting file to the output dir
            break;
        }
    }

    // #todo cleanup tmp files
    return 0;
}
