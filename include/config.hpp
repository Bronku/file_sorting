#pragma once
#include <getopt.h>
#include <string>

class Configuration {
public:
    // I/O settings
    std::string input_file = "1.in";
    std::string output_file = "1.out";
    std::string tmp_directory = "run";

    // Algorithm settings
    size_t buffer_rows = 101;
    size_t buffer_cols = 10;

    // Additional modes
    size_t generate_data = 0;
    bool read_file = false;

    Configuration(int argc, char** argv);
};
