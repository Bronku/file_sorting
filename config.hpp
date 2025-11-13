#pragma once
#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>

class Configuration {
public:
    std::string input_file = "1.in";
    std::string output_file = "1.in";
    std::string directory = "tmp";
    bool generate_data = false;
    int N = 100000; // number of records in a file
    int n = 101; // number of buffers
    int b = 10; // blocking factor

    static Configuration parse_args(int argc, char** argv)
    {
        if (argc <= 1) {
            std::cerr << "Please specify input arguments\n";
            std::cerr << "Usage: " << argv[0] << " [-g] [-N count] [-i input] [-o output] [-d directory] [-n buffers] [-b block_size]\n";
            throw std::invalid_argument("No input arguments");
        }
        Configuration config;
        bool input_set = false;
        int opt;

        while ((opt = getopt(argc, argv, "i:o:d:gN:n:b:")) != -1) {
            switch (opt) {
            case 'i':
                config.input_file = optarg;
                input_set = true;
                break;
            case 'o':
                config.output_file = optarg;
                break;
            case 'd':
                config.directory = optarg;
                break;
            case 'g':
                config.generate_data = true;
                break;
            case 'N':
                config.N = std::stoi(optarg);
                break;
            case 'n':
                config.n = std::stoi(optarg);
                break;
            case 'b':
                config.b = std::stoi(optarg);
                break;
            case '?':
                throw std::invalid_argument("Invalid command line argument");
            }
        }

        if (config.generate_data && input_set) {
            throw std::invalid_argument("Cannot specify both -g and -i options");
        }

        return config;
    }
};
