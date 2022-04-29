//
// Created by bogum on 10.04.2022.
//

#include <iostream>
#include "clean.h"
#include "../components/Location.h"
#include "../utils/FileMaker.h"

namespace test {
    void test_out() {
        std::cout << "test_out" << std::endl;
    }
    namespace in {
        void test_in() {
            std::cout << "test_in" << std::endl;
        }
    }
}


int main_cpp() {
    FileMaker::test();
    return 0;
};