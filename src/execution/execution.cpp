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

bool kokkos_arg = false;
bool kokkos_team = false;
bool kokkos_test = true;

// global view array size factor
int N0;
int safety = INT_MIN;
int stride_voxel;

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

struct quantizationTest {
    Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView;
    Face* faces;

    KOKKOS_INLINE_FUNCTION
    explicit
    quantizationTest(Face *faces, Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView_) noexcept
            : faces(faces), voxelView(voxelView_) {}

    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        std::list<Voxel> voxelList;
        algorithm::quantizationAlgorithmKokkos(faces, algorithm::getTessellationLevels(faces[i]), i, &voxelList);
        // check if j is in view size
        if (voxelList.size() > stride_voxel) {
            throw std::runtime_error("voxel index is out of View size " + std::to_string(voxelList.size()));
        }
        for (int j = 0; !voxelList.empty(); j++) {
            Voxel voxel = voxelList.front();
            voxelList.pop_front();
            voxelView(i * stride_voxel + j, 0) = voxel.getX();
            voxelView(i * stride_voxel + j, 1) = voxel.getY();
            voxelView(i * stride_voxel + j, 2) = voxel.getZ();
        }
    }
};

struct initView {
    Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView;

    KOKKOS_INLINE_FUNCTION
    explicit
    initView(Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView_) noexcept
            : voxelView(voxelView_) {}

    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        voxelView(i, 0) = safety;
        voxelView(i, 1) = safety;
        voxelView(i, 2) = safety;
    }
};

struct QuantizationTeam {

};

int algorithm::main(int argc, char *argv[]) {

    if (!kokkos_arg && !kokkos_team && !kokkos_test) {
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

    if (kokkos_test) {
        char **kokkosArgs = new char *[argc + 1];
        for (int i = 0; i < argc; i++) {
            kokkosArgs[i] = argv[i];
        }
        kokkosArgs[argc] = (char *) &"";
        int kokkosArgc = argc + 1;
        Kokkos::initialize(kokkosArgc, kokkosArgs);

        // Load model from .obj file
        auto *objectStore = new ObjectStore();
        fileMaker::loadObject("hand_02.obj", objectStore);
        std::cout << objectStore->getSizeFaces() << "\n";
        auto faces = objectStore->getFaces();
        // TODO: needs to fix mocked version to dynamically set size according to faces size
        int tes_max = 0;
        std::for_each(faces, faces + objectStore->getSizeFaces(), [&tes_max](Face &face) {
            int tes = getTessellationLevels(face);
            if (tes > tes_max) {
                tes_max = tes;
            }
        });

        stride_voxel = (int) pow(4, tes_max) * 3;
        N0 = (int) (objectStore->getSizeFaces() * stride_voxel);
        Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView ("VoxelStoreDuplicates", N0);
        std::cout << "stride_voxel: " << stride_voxel << "\n";
        // initializes view with -1 in values
        Kokkos::parallel_for(N0, initView(voxelView));

        // quantization
        Kokkos::parallel_for(objectStore->getSizeFaces() - 1, quantizationTest(faces, voxelView));
        Kokkos::fence();

        // write voxels from View to VoxelStore
        std::deque<Voxel> voxels;
        for (int i = 0; i < N0; i++) {
            if (voxelView(i, 0) != safety && voxelView(i, 1) != safety && voxelView(i, 2) != safety) {
                voxels.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
            }
        }
        auto* voxelStore = new VoxelStore();
        voxelStore->setVoxelsDeque(voxels);
        voxelStore->convertToArray();
        fileMaker::saveSchematic("hand_02", voxelStore);

        Kokkos::finalize();
    }


    return 0;
}
