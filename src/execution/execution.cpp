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
#include "Kokkos_Sort.hpp"

bool kokkos_arg = false;
bool kokkos_team = false;
bool kokkos_test = false;

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
    int* start_point;
    Face* faces;

    KOKKOS_INLINE_FUNCTION
    explicit
    quantizationTest(Face *faces_, int *start_point_, Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView_) noexcept
            : faces(faces_), start_point(start_point_), voxelView(voxelView_) {}

    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        int start = start_point[i];
        std::list<Voxel> voxelList;
        algorithm::quantizationAlgorithmKokkos(faces, algorithm::getTessellationLevels(faces[i]), i, &voxelList);
        // check if j is in view size
        if (voxelList.size() > N0) {
            throw std::runtime_error("voxel index is out of View size " + std::to_string(voxelList.size()));
        }
        for (int j = 0; !voxelList.empty(); j++) {
            Voxel voxel = voxelList.front();
            voxelList.pop_front();
            voxelView(start + j, 0) = voxel.getX();
            voxelView(start + j, 1) = voxel.getY();
            voxelView(start + j, 2) = voxel.getZ();
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


struct voxelDeduplicate {

    Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView;
    int* start_point;
    std::deque<Voxel> *voxels_deque;

    KOKKOS_INLINE_FUNCTION
    explicit voxelDeduplicate(Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView_, int *start_point_, std::deque<Voxel> *voxelDeque_) noexcept
            : start_point(start_point_), voxels_deque(voxelDeque_), voxelView(voxelView_) {}

    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        int start = start_point[i];
        auto voxel_deque = voxels_deque[i];
        auto vmap = std::map<int, std::map<int, std::map<int, int>>>();
        int deque_size = (int) voxel_deque.size();
        int temp_size = 0;
        for (int j = 0; j < deque_size; j++) {
            auto temp_v = voxel_deque.at(j);
            if (vmap.find(temp_v.getX()) == vmap.end()) {
                vmap[temp_v.getX()] = std::map<int, std::map<int, int>>();
            }
            if (vmap[temp_v.getX()].find(temp_v.getY()) == vmap[temp_v.getX()].end()) {
                vmap[temp_v.getX()][temp_v.getY()] = std::map<int, int>();
            }
            if (vmap[temp_v.getX()][temp_v.getY()].find(temp_v.getZ()) == vmap[temp_v.getX()][temp_v.getY()].end()) {
                vmap[temp_v.getX()][temp_v.getY()][temp_v.getZ()] = temp_v.getColor();
            } else {
                continue;
            }
            voxelView(start + temp_size, 0) = temp_v.getX();
            voxelView(start + temp_size, 1) = temp_v.getY();
            voxelView(start + temp_size, 2) = temp_v.getZ();
            temp_size++;
        }
        std::cout << "Size: " << temp_size << std::endl;
    }

};

struct QuantizationTeam {

};

int algorithm::main(int argc, char *argv[]) {
    int scale = 2;
    kokkos_test = true;

    if (!kokkos_arg && !kokkos_team && !kokkos_test) {
//        auto start = std::chrono::high_resolution_clock::now();
        Kokkos::Timer timer;
        double start = timer.seconds();
        auto *objectStore = new ObjectStore();
        auto *voxelStore = new VoxelStore();
        fileMaker::loadObject("hand_02.obj", objectStore, scale);
        auto faces = objectStore->getFaces();
        double obj = timer.seconds();
        std::cout << "Object loaded in " << obj - start << " seconds" << std::endl;
        // TODO: putVoxel method causes problems with duplicate checking
        for (int i = 0; i < objectStore->getSizeFaces() - 1; i++) {
//        std::cout << faces[i] << "\n";
            algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore);
        }
        double quant = timer.seconds();
        std::cout << "Quantization done in " << quant - obj << " seconds" << std::endl;
        voxelStore->convertToArray();
        double convert = timer.seconds();
        std::cout << "Conversion done in " << convert - quant << " seconds" << std::endl;
        fileMaker::saveSchematic("mickey_mouse", voxelStore);
        double stop = timer.seconds();
        std::cout << "Schematic saved in " << stop - convert << " seconds" << std::endl;
        std::cout << "Total time: " << stop - start << "\n";
//        auto stop = std::chrono::high_resolution_clock::now();
//        auto duration = stop - start;
//        std::cout << duration.count() << std::endl;
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

        Kokkos::View<Voxel*, Kokkos::LayoutRight, Kokkos::HostSpace> execution_space("execution_space", N0);
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

        // timer
        Kokkos::Timer timer;
        double start = timer.seconds();

        // Load model from .obj file
        auto *objectStore = new ObjectStore();
        fileMaker::loadObject("hand_02.obj", objectStore, scale);
//        std::cout << objectStore->getSizeFaces() << "\n";
        auto faces = objectStore->getFaces();
        auto faces_size = objectStore->getSizeFaces();
        int start_point[faces_size];
        double after_obj = timer.seconds();
        std::cout << "Object load time: " << after_obj - start << "\n";
        // TODO: needs to fix mocked version to dynamically set size according to faces size
        double vox_total = 0;
        int temp_vox = 0;
        for (int i = 0; i < faces_size - 1; i++) {
            int tes = algorithm::getTessellationLevels(faces[i]);
            int voxel_change = pow(4, tes) * 3;
            vox_total += voxel_change;
            start_point[i] = temp_vox;
            temp_vox += voxel_change;
        }
//        std::for_each(faces, faces + objectStore->getSizeFaces(), [&tes_max](Face &face) {
//            int tes = getTessellationLevels(face);
//            if (tes > tes_max) {
//                tes_max = tes;
//            }
//        });

        double after_tes = timer.seconds();
        std::cout << "Time of finding max teselation: " << after_tes - after_obj << "\n";
//        stride_voxel = (int) pow(4, vox_total) * 3;
//        N0 = (int) (objectStore->getSizeFaces() * stride_voxel);
        N0 = (int) vox_total;
        std::cout << "Total voxels: " << N0 << "\n";
        Kokkos::View<int*[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView ("VoxelStoreDuplicates", N0);

        double after_view = timer.seconds();
        std::cout << "Time of View allocation: " << after_view - after_tes << "\n";
        Kokkos::parallel_for(N0, initView(voxelView));
        double after_init = timer.seconds();
        std::cout << "Time of View initiation: " << after_init - after_view << "\n";
        // quantization
        Kokkos::parallel_for(objectStore->getSizeFaces() - 1, quantizationTest(faces, start_point, voxelView));
        Kokkos::fence();
        double after_quant = timer.seconds();
        std::cout << "Time of quantization: " << after_quant - after_init << "\n";

        // write voxels from View to VoxelStore
        std::deque<Voxel> voxels;
        std::deque<Voxel> temp_voxels_deque_1;
        std::deque<Voxel> temp_voxels_deque_2;
        std::deque<Voxel> temp_voxels_deque_3;
        std::deque<Voxel> temp_voxels_deque_4;
        int deque_start_points[4] = {0, (int) std::floor(N0 / 4), (int) std::floor(N0 / 4) * 2, (int) std::floor(N0 / 4) * 3};
        for (int i = 0; i < N0; i++) {
            if (voxelView(i, 0) != safety && voxelView(i, 1) != safety && voxelView(i, 2) != safety) {
//                voxels.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
                if (i <= (int) std::floor(N0 / 4)) {
                    temp_voxels_deque_1.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
                } else if (i <= (int) std::floor(N0 / 4) * 2) {
                    temp_voxels_deque_2.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
                } else if (i <= (int) std::floor(N0 / 4) * 3) {
                    temp_voxels_deque_3.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
                } else {
                    temp_voxels_deque_4.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
                }
                voxelView(i, 0) = safety;
                voxelView(i, 1) = safety;
                voxelView(i, 2) = safety;
            }
        }
        double loop = timer.seconds();
        std::deque<Voxel> temp_voxels[4] = {temp_voxels_deque_1, temp_voxels_deque_2, temp_voxels_deque_3, temp_voxels_deque_4};
        // TODO: mocked thread count
        Kokkos::parallel_for(4, voxelDeduplicate(voxelView, deque_start_points, temp_voxels));
        std::cout << "Time of loop: " << loop - after_quant << "\n";
        auto* voxelStore = new VoxelStore();
//        std::deque<Voxel> temp_voxels_deque_3;
//        std::deque<Voxel> temp_voxels_deque_4;
        // try to split into block and remove duplicates then merge
        // or try to give thread its memory view
        for (int i = 0; i < N0; i++) {
            if (voxelView(i, 0) != safety && voxelView(i, 1) != safety && voxelView(i, 2) != safety) {
                voxels.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
            }
        }
        voxelStore->setVoxelsDeque(voxels);
        voxelStore->convertToArray();
        std::cout << "Total voxels in deque: " << voxels.size() << " || in View: " << N0 << "\n";
        double end = timer.seconds();
        std::cout << "Time of converting View to VoxelStore: " << end - loop << "\n";
        fileMaker::saveSchematic("hand_02", voxelStore);

        std::cout << "Time: " << end - start << "\n";

        std::cout << "last voxel: " << voxelView(104583 - 1, 0) << " " << voxelView(104583 - 1, 1) << " " << voxelView(104583 - 1, 2) << "\n";
        std::cout << "last voxel: " << voxelView((N0 / 2) - 1, 0) << " " << voxelView((N0 / 2) - 1, 1) << " " << voxelView((N0 / 2) - 1, 2) << "\n";
        std::cout << "last voxel: " << voxelView(N0 - 1, 0) << " " << voxelView(N0 - 1, 1) << " " << voxelView(N0 - 1, 2) << "\n";

        Kokkos::finalize();
    }


    return 0;
}
