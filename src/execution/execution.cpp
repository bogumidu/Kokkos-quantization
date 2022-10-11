//
// Created by bogum on 11.10.2022.
//

#include <iostream>
#include <thread>
#include "Kokkos_Core.hpp"
#include "execution.h"
#include "../utils/ObjectStore.h"
#include "../utils/FileMaker.h"

struct hello_world {

    int n;

    KOKKOS_INLINE_FUNCTION
    explicit
    hello_world(int num) noexcept
            : n(num)
    { }

    // this is a macro that allows method to run on CUDA devices when using CUDA
    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        printf("N is %d, Hello from i = %i\n", n, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

int Voxelization::main(int argc, char *argv[]) {


    auto* objectStore = new ObjectStore();
    FileMaker::loadObject("hand_02.obj", objectStore);
    auto faces = objectStore->getFaces();
    auto vertices = objectStore->getVertices();
    auto size_vertices = objectStore->getSizeVertices();
    std::cout << faces << " <- is a location of faces, \n" << vertices << ": " << size_vertices << std::endl;
//    std::cout << argc << ":\n";
//    std::string all_args[argc];
//    bool debug;
//
//    if (argc < 2) throw std::runtime_error("Missing object file name");
//
//
//    if (argc > 2) {
//        for (int i = 2; i <= argc - 1; i++) {
//            all_args[i] = argv[i];
//        }
//    }
//    auto voxels = FileMaker::loadFile::loadSchematic("test1.schematic");
//    for (const auto& voxel : voxels) {
//        std::cout << voxel.getX() << " " << voxel.getY() << " " << voxel.getZ() << " " << voxel.getColor() << std::endl;
//    }
//    FileMaker::loadFile::saveSchematic("test2.schematic", voxels);
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
