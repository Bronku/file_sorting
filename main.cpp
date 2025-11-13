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
    std::ofstream out_stream(filename);
    Writer output(out_stream);

    for (int i = 0; i < N; i++) {
        Record rec = Record::random();
        output.write(rec);
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

        sort_file(opts);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
