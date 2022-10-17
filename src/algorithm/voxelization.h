//
// Created by bogum on 11.10.2022.
//

#ifndef PROJECT2_VOXELIZATION_H
#define PROJECT2_VOXELIZATION_H

#include "../components/Voxel.h"
#include "../components/TempFace.h"
#include "../utils/VoxelStore.h"

namespace voxelization {
    inline namespace teselation {
        Voxel* quantizationAlgorithm(TempFace* face, int accuracy, Voxel* voxel, VoxelStore* voxelStore);
        int getTeselationLevels(Face* face);
    }
    inline namespace voxelization {
        Voxel* createVoxel(TempFace* face, Voxel* voxel, VoxelStore* voxelStore);
        Voxel* storeVoxel(Voxel* voxel, VoxelStore* voxelStore);
    }
    inline namespace utils {
        // TODO: change name
        bool almostEqual(std::vector<double> v1, std::vector<double> v2);
    }
}

#endif //PROJECT2_VOXELIZATION_H
