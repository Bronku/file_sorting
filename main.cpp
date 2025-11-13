#include "config.hpp"
#include "reader.hpp"
#include "record.hpp"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

int generate_file(int N, const std::string& filename)
{
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open output file: " << filename << '\n';
        return 1;
    }

    for (int i = 0; i < N; i++) {
        Record rec = Record::random();
        out << rec << '\n';
    }

    return 0;
}

int sort_file(const Configuration& opts)
{
    // TODO: Implement sorting with new buffer classes
    std::cout << "Sorting not yet implemented\n";
    return 0;
}

int main(int argc, char** argv)
{
    try {
        Configuration opts = Configuration::parse_args(argc, argv);

        if (opts.generate_data) {
            return generate_file(opts.N, opts.output_file);
        }

        return sort_file(opts);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
