#include <iostream>
#include "execution/execution.h"

int scale = 12;

int main(int argc, char *argv[]) {

    // for storing add list wit push back
    // make it as a VerticesStore with method putVertex so no duplicates

//    int a = 5;
//    auto *ptr = &a;
//    std::cout << ptr << "\n";

    // run configuration
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << "\n";
    }



    execution::main(argc, argv);

    return 0;
}


