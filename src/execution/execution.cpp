//
// Created by bogum on 11.10.2022.
//

#include <iostream>
#include <thread>
#include <utility>
#include "Kokkos_Core.hpp"
#include "execution.h"
#include "../utils/ObjectStore.h"
#include "../utils/fileMaker.h"
#include "../algorithm/algorithm.h"

bool kokkos_arg = true;

struct quantization {

    Kokkos::View<Voxel*, Kokkos::LayoutRight, Kokkos::HostSpace> sh_mem;
    Face *faces;

    KOKKOS_INLINE_FUNCTION
    explicit
    quantization(Face *faces, Kokkos::View<Voxel*, Kokkos::LayoutRight, Kokkos::HostSpace> sh_mem_) noexcept
            : faces(faces), sh_mem(sh_mem_) {}

    // this is a macro that allows method to run on CUDA devices when using CUDA
    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        auto voxelStore = new VoxelStore();
        algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore);
        auto voxels_deque = voxelStore->getVoxelsDeque();
        for (int j = 0; j < voxels_deque.size(); j++) {
            sh_mem(j) = voxels_deque.at(j);
        }
    }
};

int algorithm::main(int argc, char *argv[]) {

    if (!kokkos_arg) {
        auto *objectStore = new ObjectStore();
        auto *voxelStore = new VoxelStore();
        fileMaker::loadObject("hand_02.obj", objectStore);
        auto faces = objectStore->getFaces();
        // TODO: putVoxel method causes problems with duplicate checking
        for (int i = 0; i < objectStore->getSizeFaces() - 1; i++) {
//        std::cout << faces[i] << "\n";
            algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore);
        }
        voxelStore->convertToArray();
        fileMaker::saveSchematic("hand_02", voxelStore);
    }

//    auto voxels = fileMaker::loadFile::loadSchematic("test1.schematic");
//    for (const auto& voxel : voxels) {
//        std::cout << voxel.getX() << " " << voxel.getY() << " " << voxel.getZ() << " " << voxel.getColor() << std::endl;
//    }
//
    // TODO: parralel for kookkos method does not work bcs it tries to access local memopry of other threads
    if (kokkos_arg) {
        char **kokkosArgs = new char *[argc + 1];
        for (int i = 0; i < argc; i++) {
            kokkosArgs[i] = argv[i];
        }
        kokkosArgs[argc] = (char *) &"";
        int kokkosArgc = argc + 1;
        Kokkos::initialize(kokkosArgc, kokkosArgs);

        Kokkos::View<Voxel*, Kokkos::LayoutRight, Kokkos::HostSpace> execution_space("execution_space", 4);
        auto *objectStore = new ObjectStore();
        auto *voxelStore = new VoxelStore();
        fileMaker::loadObject("hand_02.obj", objectStore);
        auto faces = objectStore->getFaces();
        Kokkos::DefaultExecutionSpace execution;

        printf("Hello World on Kokkos execution space %s\n",
               typeid(Kokkos::DefaultExecutionSpace).name());

        // executes function x amount of times
        Kokkos::parallel_for(objectStore->getSizeFaces() - 1, quantization(faces, execution_space));
        Kokkos::fence();

        Kokkos::finalize();
    }


    return 0;
}
