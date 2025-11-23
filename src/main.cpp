#include "../include/config.hpp"
#include "../include/file_reader.hpp"
#include "../include/file_writer.hpp"
#include "../include/merge_sorter.hpp"
#include "../include/record.hpp"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

void generate_file(int N, const std::string& filename)
{
    FileWriter output(filename);

    for (int i = 0; i < N; i++) {
        Record rec = Record::random();
        output.write(rec);
    }
}

void read_and_evaluate(std::string filename)
{
    FileReader input(filename);
    for (size_t i = 0;; i++) {
        auto rec = input.read();
        if (!rec.has_value()) {
            return;
        }

        std::cout << "record " << i << " [value: " << rec.value().evaluate() << "] " << rec.value() << "\n";
    }
}

int main(int argc, char** argv)
{
    Configuration opts(argc, argv);

    if (opts.generate_data) {
        generate_file(opts.generate_data, opts.output_file);
        return 0;
    }

    if (opts.read_file) {
        read_and_evaluate(opts.input_file);
        return 0;
    }

    MergeSorter sorter(opts.buffer_rows, opts.buffer_cols, opts.tmp_directory);
    sorter.sort_file(opts.input_file, opts.output_file);
    std::cout << "total reads: " << sorter.disk_reads() << '\n'
              << "total writes: " << sorter.disk_writes() << '\n'
              << "phases: " << sorter.phases() << "\n";
}
