#pragma once
#include <getopt.h>
#include <string>

class Configuration {
public:
    std::string input_file = "1.in";
    std::string output_file = "1.out";
    std::string tmp_dir = "build/run";
    bool generate_data = false;
    bool evaluate_file = false;
    int N = 100000; // number of records in a file
    int n = 101; // number of buffers
    int b = 10; // blocking factor

    Configuration(int argc, char** argv);
};
