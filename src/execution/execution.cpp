//
// Created by bogum on 11.10.2022.
//

#include <iostream>
#include <thread>
#include "Kokkos_Core.hpp"
#include "execution.h"
#include "../utils/ObjectStore.h"
#include "../utils/fileMaker.h"
#include "../algorithm/algorithm.h"

//struct hello_world {
//
//    int n;
//
//    KOKKOS_INLINE_FUNCTION
//    explicit
//    hello_world(int num) noexcept
//            : n(num)
//    { }
//
//    // this is a macro that allows method to run on CUDA devices when using CUDA
//    KOKKOS_INLINE_FUNCTION
//    void operator()(const int i) const {
//        printf("N is %d, Hello from i = %i\n", n, i);
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }
//};

int algorithm::main(int argc, char *argv[]) {


    auto* objectStore = new ObjectStore();
//    auto* voxelStore = new VoxelStore();
    fileMaker::loadObject("hand_02.obj", objectStore);
    auto faces = objectStore->getFaces();
    for (int i = 0; i <= 50; i++) {
        algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore)
    }
//    auto voxels = fileMaker::loadFile::loadSchematic("test1.schematic");
//    for (const auto& voxel : voxels) {
//        std::cout << voxel.getX() << " " << voxel.getY() << " " << voxel.getZ() << " " << voxel.getColor() << std::endl;
//    }
//    fileMaker::loadFile::saveSchematic("test2.schematic", voxels);
//    char** kokkosArgs = new char*[argc + 1];
//    for (int i = 0; i < argc; i++) {
//        kokkosArgs[i] = argv[i];
//    }
//    kokkosArgs[argc] = (char *) &"";
//    int kokkosArgc = argc + 1;
//    Kokkos::initialize(kokkosArgc, kokkosArgs);
//
//
//    printf("Hello World on Kokkos execution space %s\n",
//           typeid(Kokkos::DefaultExecutionSpace).name());
//
//    // executes function x amount of times
//    Kokkos::parallel_for("HelloWorld", 15, hello_world(5));
//
//    Kokkos::finalize();


    return 0;
}
