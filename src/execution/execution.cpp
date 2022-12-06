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

struct quantization {

    Face* faces;
    VoxelStore *voxelStore;

    KOKKOS_INLINE_FUNCTION
    explicit
    quantization(Face* faces,VoxelStore* voxelStore) noexcept
            : faces(faces), voxelStore(voxelStore)
    { }

    // this is a macro that allows method to run on CUDA devices when using CUDA
    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore);
    }
};

int algorithm::main(int argc, char *argv[]) {


    auto* objectStore = new ObjectStore();
    auto* voxelStore = new VoxelStore();
    fileMaker::loadObject("hand_02.obj", objectStore);
    auto faces = objectStore->getFaces();
    // TODO: putVoxel method causes problems with duplicate checking
    for (int i = 0; i < objectStore->getSizeFaces() - 1; i++) {
//        std::cout << faces[i] << "\n";
        algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore);
    }
    voxelStore->convertToArray();
    fileMaker::saveSchematic("hand_02", voxelStore);

//    auto voxels = fileMaker::loadFile::loadSchematic("test1.schematic");
//    for (const auto& voxel : voxels) {
//        std::cout << voxel.getX() << " " << voxel.getY() << " " << voxel.getZ() << " " << voxel.getColor() << std::endl;
//    }
//
    // TODO: parralel for kookkos method does not work bcs it tries to access local memopry of other threads
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
//    Kokkos::parallel_for("HelloWorld", objectStore->getSizeFaces() - 1, quantization(faces, voxelStore));
//
//    Kokkos::finalize();


    return 0;
}
