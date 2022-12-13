//
// Created by bogum on 14.04.2022.
//

#ifndef PROJECT2_FILEMAKER_H
#define PROJECT2_FILEMAKER_H

#include <list>
#include <vector>
#include "../components/Voxel.h"
#include "ObjectStore.h"
#include "VoxelStore.h"

namespace fileMaker {

    inline namespace testFM {
        void test();
    }

    inline namespace loadFile{
        void loadObject(const std::string& fileName, ObjectStore* objectStore, int ratio = 1);
        std::vector<Voxel> loadSchematic(const std::string& fileName);
//        void assignPropperites(float *maxX) { *maxX = maxX; }
        void saveSchematic(const std::string &fileName, VoxelStore *voxelStore);
    }

    inline namespace utils {
        std::list<std::string> split(std::string &str, char delimiter);

        unsigned short readUnsignedShort(std::ifstream &input);

        int readRGB(std::ifstream &input);

        void writeUnsignedShort(std::ofstream &output, unsigned short v);

        void writeRGB(std::ofstream &output, int v);

        void preprocessVoxels(Voxel *voxels, int voxels_size);

        int parseInt(const std::string& s);
    }
}
#endif //PROJECT2_FILEMAKER_H
