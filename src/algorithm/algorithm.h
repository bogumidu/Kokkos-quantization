//
// Created by bogum on 11.10.2022.
//

#ifndef PROJECT2_ALGORITHM_H
#define PROJECT2_ALGORITHM_H

#include "../components/Voxel.h"
#include "../utils/VoxelStore.h"
#include "../components/Face.h"

namespace algorithm {
    // main algorithm functor
    void quantizationAlgorithm(Face *faces, int accuracy, int index, VoxelStore *voxelStore);

    // tessellation related functors
    inline namespace tessellation {
        int getTessellationLevels(Face face);
    }

    inline namespace utils {
        bool almostEqual(std::vector<double> v1, std::vector<double> v2);
        double distance(std::vector<double> v1, std::vector<double> v2);
    }
}

#endif //PROJECT2_ALGORITHM_H