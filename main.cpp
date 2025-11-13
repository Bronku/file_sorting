#include "config.hpp"
#include "record.hpp"
#include "sort.cpp"
#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

void generate_file(int N, const std::string& filename)
{
    std::filesystem::path dirPath = std::filesystem::path(filename).parent_path();
    std::filesystem::create_directories(dirPath);

    std::ofstream out_stream(filename);
    Writer output(out_stream);

    for (int i = 0; i < N; i++) {
        Record rec = Record::random();
        output.write(rec);
    }
}

void read_and_evaluate(std::string filename, int n)
{
    Buffer main_buffer(n);
    auto buff = main_buffer.divide(1)[0];

    std::ifstream in_stream(filename);
    Reader input_reader(in_stream);

    while (true) {
        size_t records_read = read_chunk(input_reader, buff);

        if (records_read == 0) {
            break;
        }

        for (size_t i = 0; i < records_read; ++i) {
            const Record& rec = buff[i];
            std::cout << "[Evaluate i = " << i << ": " << rec.evaluate() << "] " << rec << "\n";
        }
    }
}

int main(int argc, char** argv)
{
    try {
        Configuration opts = Configuration::parse_args(argc, argv);

        if (opts.generate_data) {
            generate_file(opts.N, opts.output_file);
            return 0;
        }

        if (opts.evaluate_file) {
            read_and_evaluate(opts.input_file, opts.N);
            return 0;
        }

        sort_file(opts);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
