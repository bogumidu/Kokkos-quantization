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
//#include <execution>

bool kokkos_arg = false;

// global view array size factor
int N0;
int safety = INT_MIN;

struct quantization {

    Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView;
    int *start_point;
    Face *faces;

    KOKKOS_INLINE_FUNCTION
    explicit
    quantization(Face *faces_, int *start_point_,
                 Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView_) noexcept
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
    Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView;

    KOKKOS_INLINE_FUNCTION
    explicit
    initView(Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView_) noexcept
            : voxelView(voxelView_) {}

    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
        voxelView(i, 0) = safety;
        voxelView(i, 1) = safety;
        voxelView(i, 2) = safety;
    }
};


struct voxelDeduplicate {

    Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView;
    int *start_point;
    std::deque<Voxel> *voxels_deque;

    KOKKOS_INLINE_FUNCTION
    explicit voxelDeduplicate(Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView_,
                              int *start_point_, std::deque<Voxel> *voxelDeque_) noexcept
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
    }

};


int execution::main(int argc, char *argv[]) {
    int scale = 30;
    kokkos_arg = true;
    int thread_count = 8;

    /**
     * Console arguments processing
     */

    char **kokkosArgs = new char *[argc + 1];
    for (int i = 0; i < argc; i++) {
        kokkosArgs[i] = argv[i];
        std::cout << kokkosArgs[i] << std::endl;
    }
    kokkosArgs[argc] = (char *) &"";
    int kokkosArgc = argc + 1;

    if (!kokkos_arg) {

        Kokkos::Timer timer;
        double start = timer.seconds();

        /**
         * Container objects initialization
         * Load object into ObjectStore container
         * faces - face array
         */

        auto *objectStore = new ObjectStore();
        auto *voxelStore = new VoxelStore();
        fileMaker::loadObject("skull.obj", objectStore, scale);
        auto faces = objectStore->getFaces();
        double obj = timer.seconds();
        std::cout << "Object loaded in " << obj - start << " seconds" << std::endl;

        /**
         * Sequential quantization with recursion
         */

        for (int i = 0; i < objectStore->getSizeFaces() - 1; i++) {
            algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore);
        }
        double quant = timer.seconds();
        std::cout << "Quantization done in " << quant - obj << " seconds" << std::endl;

        /**
         * Deduplicate voxels
         */

        voxelStore->convertToArray();
        double convert = timer.seconds();
        std::cout << "Conversion done in " << convert - quant << " seconds" << std::endl;

        /**
         * Save voxels to file
         */

        fileMaker::saveSchematic("mickey_mouse", voxelStore);
        double stop = timer.seconds();
        std::cout << "Schematic saved in " << stop - convert << " seconds" << std::endl;
        std::cout << "Total time: " << stop - start << "\n";
    }

    if (kokkos_arg) {


        Kokkos::initialize(Kokkos::InitializationSettings().set_num_threads(thread_count));
        {
            Kokkos::Timer timer;
            double start_program = timer.seconds();

            /**
             * Container objects initialization
             * Load object into ObjectStore container
             * faces - face array before tessellation
             * start_point - array of face start index after tessellation
             * vox_total - calculated size of voxelView
             */

            auto *objectStore = new ObjectStore();
            fileMaker::loadObject("skull.obj", objectStore, scale);
            auto faces = objectStore->getFaces();
            auto faces_size = objectStore->getSizeFaces();
            int start_point[faces_size];
            double after_obj = timer.seconds();
            double vox_total = 0;
            int temp_vox = 0;
            for (int i = 0; i < faces_size - 1; i++) {
                int tes = algorithm::getTessellationLevels(faces[i]);
                int voxel_change = (int) pow(4, tes) * 3;
                vox_total += voxel_change;
                start_point[i] = temp_vox;
                temp_vox += voxel_change;
            }

            if (vox_total < 1000000) std::cout << "Small voxel count may lead to inaccurate results. Calculated voxel count is: " << vox_total << "\n";

            /**
             * View initialization
             * N0 - calculated size of voxelView
             * Kokkos::parallerl_for(N0, initView(voxelView)) - fill voxelView with INT_MIN value
             */

            N0 = (int) vox_total;
            Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView("VoxelStoreDuplicates", N0);
            Kokkos::parallel_for(N0, initView(voxelView));

            /**
             * quantization
             * Kokkos::parallel_for(objectStore->getSizeFaces() - 1, quantization(faces, start_point, voxelView));
             * objectStore->getSizeFaces() - 1 - size of faces array
             * quantization(faces, start_point, voxelView) - quantization functor
             * faces - faces array
             * start_point - array of index shifts for faces after teselation
             * voxelView - storage for voxels
             * Kokkos::fence(); - ensures parallel calculations are finished
             */

            double start_quant = timer.seconds();
            Kokkos::parallel_for(objectStore->getSizeFaces() - 1, quantization(faces, start_point, voxelView));
            Kokkos::fence();
            double stop_quant = timer.seconds();
            std::cout << "Time of quantization: " << stop_quant - start_quant << "\n";

            /**
             * Voxels deduplication algorithm
             * Split voxelView into thread_count of voxel std::deque
             * Kokkos::parallel_for(thread_count, voxelDeduplicate(voxelView, deque_start_points, temp_voxels));
             * thread_count - number of std::deque in temp_voxels array
             * voxelDeduplicate(voxelView, deque_start_points, temp_voxels)); - deduplication functor
             * voxelView - storage for voxels
             * deque_start_points - array of start index in voxelView
             * temp_voxels - array of voxel std::deque
             */

            double start_dedup = timer.seconds();
            auto* temp_voxels = new std::deque<Voxel>[thread_count];
            int deque_size = std::floor(N0 / thread_count);
            int index = 0;
            auto* deque_start_points = new int[thread_count];
            for (int i = 0; i < thread_count; i++) {
                deque_start_points[i] = deque_size * i;
                for (int j = 0; j < deque_size; j++) {
                    if (voxelView(index, 0) != safety && voxelView(index, 1) != safety && voxelView(index, 2) != safety) {
                        temp_voxels[i].emplace_back(voxelView(index, 0), voxelView(index, 1), voxelView(index, 2), 1);
                        voxelView(index, 0) = safety;
                        voxelView(index, 1) = safety;
                        voxelView(index, 2) = safety;
                    }
                    index++;
                }
            }


            Kokkos::parallel_for(thread_count, voxelDeduplicate(voxelView, deque_start_points, temp_voxels));
            Kokkos::fence();
            double stop_dedup = timer.seconds();
            std::cout << "Time of deduplication: " << stop_dedup - start_dedup << "\n";

            /**
             * voxels - final voxel std::deque
             */

            std::deque<Voxel> voxels;
            auto *voxelStore = new VoxelStore();
            for (int i = 0; i < N0; i++) {
                if (voxelView(i, 0) != safety && voxelView(i, 1) != safety && voxelView(i, 2) != safety) {
                    voxels.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
                }
            }
            voxelStore->setVoxelsDeque(voxels);
            voxelStore->convertToArray();
            std::cout << "Total voxels in deque: " << voxels.size() << " || in View: " << N0 << "\n";
            fileMaker::saveSchematic("hand_02", voxelStore);

            double end_program = timer.seconds();
            std::cout << "Total time: " << end_program - start_program << "\n";
        }
        Kokkos::finalize();
    }


    return 0;
}

void execution::runMultiCore(std::string& fileName, int thread_count, int scale, bool createLog, bool debug) {
    Kokkos::initialize(Kokkos::InitializationSettings().set_num_threads(thread_count));
    {
        Kokkos::Timer timer;
        double start_program = timer.seconds();

        /**
         * Container objects initialization
         * Load object into ObjectStore container
         * faces - face array before tessellation
         * start_point - array of face start index after tessellation
         * vox_total - calculated size of voxelView
         */

        auto *objectStore = new ObjectStore();
        fileMaker::loadObject(fileName + ".obj", objectStore, scale);
        auto faces = objectStore->getFaces();
        auto faces_size = objectStore->getSizeFaces();
        int start_point[faces_size];
        double vox_total = 0;
        int temp_vox = 0;
        for (int i = 0; i < faces_size - 1; i++) {
            int tes = algorithm::getTessellationLevels(faces[i]);
            int voxel_change = (int) pow(4, tes) * 3;
            vox_total += voxel_change;
            start_point[i] = temp_vox;
            temp_vox += voxel_change;
        }

        if (vox_total < 2000000) std::cerr
            << "WARNING: Small voxel count may lead to inaccurate results. Calculated voxel count is: "
            << vox_total << "\n";

        /**
         * View initialization
         * N0 - calculated size of voxelView
         * Kokkos::parallerl_for(N0, initView(voxelView)) - fill voxelView with INT_MIN value
         */

        N0 = (int) vox_total;
        Kokkos::View<int *[3], Kokkos::MemoryTraits<Kokkos::RandomAccess>> voxelView("VoxelStoreDuplicates", N0);
        Kokkos::fence();
        Kokkos::parallel_for(N0, initView(voxelView));
        Kokkos::fence();

        /**
         * quantization
         * Kokkos::parallel_for(objectStore->getSizeFaces() - 1, quantization(faces, start_point, voxelView));
         * objectStore->getSizeFaces() - 1 - size of faces array
         * quantization(faces, start_point, voxelView) - quantization functor
         * faces - faces array
         * start_point - array of index shifts for faces after teselation
         * voxelView - storage for voxels
         * Kokkos::fence(); - ensures parallel calculations are finished
         */

        double start_quant = timer.seconds();
        Kokkos::parallel_for(objectStore->getSizeFaces() - 1, quantization(faces, start_point, voxelView));
        Kokkos::fence();
        double stop_quant = timer.seconds();
        if (debug) std::cout << "Time of quantization: " << stop_quant - start_quant << "\n";

        /**
         * Voxels deduplication algorithm
         * Split voxelView into thread_count of voxel std::deque
         * Kokkos::parallel_for(thread_count, voxelDeduplicate(voxelView, deque_start_points, temp_voxels));
         * thread_count - number of std::deque in temp_voxels array
         * voxelDeduplicate(voxelView, deque_start_points, temp_voxels)); - deduplication functor
         * voxelView - storage for voxels
         * deque_start_points - array of start index in voxelView
         * temp_voxels - array of voxel std::deque
         */

        double start_dedup = timer.seconds();
        auto* temp_voxels = new std::deque<Voxel>[thread_count];
        int deque_size = std::floor(N0 / thread_count);
        int index = 0;
        auto* deque_start_points = new int[thread_count];
        for (int i = 0; i < thread_count; i++) {
            deque_start_points[i] = deque_size * i;
            for (int j = 0; j < deque_size; j++) {
                if (voxelView(index, 0) != safety && voxelView(index, 1) != safety && voxelView(index, 2) != safety) {
                    temp_voxels[i].emplace_back(voxelView(index, 0), voxelView(index, 1), voxelView(index, 2), 1);
                    voxelView(index, 0) = safety;
                    voxelView(index, 1) = safety;
                    voxelView(index, 2) = safety;
                }
                index++;
            }
        }

        Kokkos::fence();
        Kokkos::parallel_for(thread_count, voxelDeduplicate(voxelView, deque_start_points, temp_voxels));
        Kokkos::fence();

        /**
         * voxels - final voxel std::deque
         */

        std::deque<Voxel> voxels;
        auto *voxelStore = new VoxelStore();
        for (int i = 0; i < N0; i++) {
            if (voxelView(i, 0) != safety && voxelView(i, 1) != safety && voxelView(i, 2) != safety) {
                voxels.emplace_back(voxelView(i, 0), voxelView(i, 1), voxelView(i, 2), 1);
            }
        }
        voxelStore->setVoxelsDeque(voxels);
        voxelStore->convertToArray();
        double stop_dedup = timer.seconds();
        if (debug) std::cout << "Time of deduplication: " << stop_dedup - start_dedup << "\n";
        if (debug) std::cout << "Total voxels in deque: " << voxels.size() << " || in View: " << N0 << "\n";
        fileMaker::saveSchematic(fileName, voxelStore);

        double end_program = timer.seconds();
        if (debug) std::cout << "Total time: " << end_program - start_program << "\n";

        if (createLog) fileMaker::generateLog(fileName, thread_count, scale, (int) voxels.size(), N0, (double) (end_program - start_program));
    }
    Kokkos::finalize();
}

void execution::runSingleCore(std::string& fileName,int scale,bool createLog,bool debug) {
    Kokkos::Timer timer;
    double start_program = timer.seconds();

    /**
     * Container objects initialization
     * Load object into ObjectStore container
     * faces - face array
     */

    auto *objectStore = new ObjectStore();
    auto *voxelStore = new VoxelStore();
    fileMaker::loadObject(fileName + ".obj", objectStore, scale);
    auto faces = objectStore->getFaces();

    /**
     * Sequential quantization with recursion
     */

    double start_quant = timer.seconds();
    for (int i = 0; i < objectStore->getSizeFaces() - 1; i++) {
        algorithm::quantizationAlgorithm(faces, algorithm::getTessellationLevels(faces[i]), i, voxelStore);
    }
    double stop_quant = timer.seconds();
    if (debug) std::cout << "Time of quantization: " << stop_quant - start_quant << "\n";

    /**
     * Deduplicate voxels
     */

    double start_dedup = timer.seconds();
    voxelStore->convertToArray();
    double stop_dedup = timer.seconds();
    if (debug) std::cout << "Time of deduplication: " << stop_dedup - start_dedup << "\n";

    /**
     * Save voxels to file
     */

    fileMaker::saveSchematic("mickey_mouse", voxelStore);
    double end_program = timer.seconds();
    if (debug) std::cout << "Total time: " << end_program - start_program << "\n";

    if (createLog) fileMaker::generateLog(fileName, 1, scale, (int) voxelStore->getSize(), 0, (double) (end_program - start_program));

}
