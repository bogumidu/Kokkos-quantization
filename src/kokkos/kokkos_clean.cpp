//
// Created by bogum on 10.04.2022.
//

#include "Kokkos_Core.hpp"
#include "kokkos_clean.h"
#include "../utils/FileMaker.h"
#include <thread>

struct hello_world {

    // this is a macro that allows method to run on CUDA devices when using CUDA
    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Hello from i = %i\n", i);
    }
};

// Probably running on single core
int main_kokkos(int argc, char *argv[]) {
    auto voxels = FileMaker::loadFile::loadSchematic("test1.schematic");
    for (const auto& voxel : voxels) {
        std::cout << voxel.getX() << " " << voxel.getY() << " " << voxel.getZ() << " " << voxel.getColor() << std::endl;
    }
    FileMaker::loadFile::saveSchematic("test2.schematic", voxels);
//    char** kokkosArgs = new char*[argc + 1];
//    for (int i = 0; i < argc; i++) {
//        kokkosArgs[i] = argv[i];
//    }
//    kokkosArgs[argc] = (char *) &"--kokkos-threads=3";
//    int kokkosArgc = argc + 1;
//    Kokkos::initialize(kokkosArgc, kokkosArgs);
//
//
//    printf("Hello World on Kokkos execution space %s\n",
//           typeid(Kokkos::DefaultExecutionSpace).name());
//
//    // executes function x amount of times
//    Kokkos::parallel_for("HelloWorld", 15, hello_world());
//
//    Kokkos::finalize();

    return 0;
}