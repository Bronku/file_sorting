#include "../include/config.hpp"
#include <iostream>
#include <stdexcept>

Configuration::Configuration(int argc, char** argv)
{
    if (argc <= 1) {
        std::cerr << "Please specify input arguments\n";
        std::cerr << "Usage: " << argv[0] << " [-g count] [-e] [-i input] [-o output] [-d dir] [-n buffers] [-b block_size]\n";
        throw std::invalid_argument("No input arguments");
    }
    bool input_set = false;
    int opt;

    while ((opt = getopt(argc, argv, "i:d:o:g:en:b:")) != -1) {
        switch (opt) {
        case 'i':
            input_file = optarg;
            input_set = true;
            break;
        case 'o':
            output_file = optarg;
            break;
        case 'd':
            tmp_directory = optarg;
            break;
        case 'g':
            generate_data = std::stoi(optarg);
            break;
        case 'e':
            read_file = true;
            break;
        case 'n':
            buffer_rows = std::stoi(optarg);
            break;
        case 'b':
            buffer_cols = std::stoi(optarg);
            break;
        case '?':
            throw std::invalid_argument("Invalid command line argument");
        default:
            std::cerr << "Please specify input arguments\n";
        }
    }

    if (generate_data && input_set) {
        throw std::invalid_argument("Cannot specify both -g and -i options");
    }
    if (generate_data && read_file) {
        throw std::invalid_argument("Cannot specify both -g and -e options");
    }
}
