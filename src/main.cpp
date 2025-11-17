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
    while (true) {
        auto rec = input.read();
        if (!rec.has_value()) {
            return;
        }

        std::cout << "[Evaluate: " << rec.value().evaluate() << "] " << rec.value() << "\n";
    }
}

int main(int argc, char** argv)
{
    Configuration opts(argc, argv);

    if (opts.generate_data) {
        generate_file(opts.N, opts.output_file);
        return 0;
    }

    if (opts.evaluate_file) {
        read_and_evaluate(opts.input_file);
        return 0;
    }

    MergeSorter sorter(opts.b, opts.n, opts.tmp_dir);
    sorter.sort_file(opts.input_file, opts.output_file);
}
