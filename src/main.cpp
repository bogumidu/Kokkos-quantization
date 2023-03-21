#include <iostream>
#include <thread>
#include "execution/execution.h"


void displayHelp();


int main(int argc, char *argv[]) {

    int thread_count = 1;
    int scale = 1;
    std::string fileName;
    bool createLog = false;
    bool debug = false;

    for (int i = 0; i < argc; ++i) {
        std::string temp_arg = argv[i];
        if (temp_arg.substr(0, 6) == "--help") {
            displayHelp();
            return 0;
        }
        if (i == 1 && temp_arg.substr(0, 1) != "-") {
            fileName = temp_arg;
            continue;
        } else if (i == 1 && temp_arg.substr(0, 1) == "-") {
            std::cerr << "First argument should be filename but provided: " << temp_arg << '\n';
            return EXIT_FAILURE;
        }
        if (temp_arg.substr(0, 3) == "-t:") {
            std::string temp_string = temp_arg.substr(3);
            thread_count = std::stoi(temp_string);
            if (thread_count < 1) {
                std::cerr << "Thread count can't be non-positive" << "\n";
                return EXIT_FAILURE;
            }
            if (thread_count > std::thread::hardware_concurrency()) {
                thread_count = (int) std::thread::hardware_concurrency();
                std::cerr << "WARNING: Provided thread count is larger than hardware thread count using "
                << thread_count << " instead" << "\n";
            }
            continue;
        }
        if (temp_arg.substr(0, 3) == "-s:") {
            std::string temp_string = temp_arg.substr(3);
            scale = std::stoi(temp_string);
            if (scale < 1) {
                std::cerr << "Scale can't be non-positive" << "\n";
                return EXIT_FAILURE;
            }
            continue;
        }
        if (temp_arg.substr(0, 5) == "--log") {
            createLog = true;
            continue;
        }
        if (temp_arg.substr(0, 3) == "--d") {
            debug = true;
            continue;
        }
    }


    std::cout << "Running voxelization for model in file: " << fileName << "\n"
    << "Set scale: " << scale << "\n"
    << "Threads provided: " << thread_count << "\n";

    if (thread_count > 1) {
        execution::runMultiCore(fileName, thread_count, scale, createLog, debug);
    } else {
        execution::runSingleCore(fileName, scale, createLog, debug);
    }


    //execution::main(argc, argv);

    return 0;
}


void displayHelp() {
    std::cout << "Usage: Kokkos-quantization [OPTIONS]\n"
              << "Options:\n"
              << "  --help                Display this help message\n"
              << "  \"fileName\"            Set file name in ../model_files folder    (needs to be passed as first argument)\n"
              << "  -t:N                  Set the number of threads used to N       (default: 1 thread)\n"
              << "  -s:N                  Set the scale to N                        (default: 1)\n"
              << "  --log                 Create log file                           (default: false)\n"
              << "  --d or --debug        Enable debug messages                     (default: false)\n";
}