#pragma once
#include "buffer.hpp"
#include "config.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include <fstream>

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

void create_initial_runs(const std::string& input_filename, const std::string& directory, Buffer& main_buffer)
{
    auto buff = main_buffer.divide(1)[0];

    std::ifstream in_stream(input_filename);
    Reader input_reader(in_stream);
    for (size_t run_index = 0;; run_index++) {
        size_t size = read_chunk(input_reader, buff);
        if (size == 0) {
            break;
        }
        std::sort(buff.begin(), buff.begin() + size);
        write_to_file(directory + "/" + std::to_string(run_index) + ".run", buff, size);
    }
}

int sort_file(const Configuration& opts)
{
    Buffer main_buffer(opts.n * opts.b);

    create_initial_runs(opts.input_file, opts.directory, main_buffer);

    return 0;
}
