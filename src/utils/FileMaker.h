//
// Created by bogum on 14.04.2022.
//

#ifndef PROJECT2_FILEMAKER_H
#define PROJECT2_FILEMAKER_H

#include <list>
#include <vector>
#include "../components/Voxel.h"
#include "ObjectStore.h"

namespace FileMaker {

    inline namespace testFM {
        void test();
    }

    inline namespace loadFile{
        void loadObject(const std::string& fileName, ObjectStore* objectStore);
        std::vector<Voxel> loadSchematic(const std::string& fileName);
//        void assignPropperites(float *maxX) { *maxX = maxX; }
        void saveSchematic(const std::string &fileName, std::vector<Voxel> &voxels);
    }

    inline namespace utils {
        std::list<std::string> split(std::string &str, char delimiter);

        unsigned short readUnsignedShort(std::ifstream &input);

        int readRGB(std::ifstream &input);

        void writeUnsignedShort(std::ofstream &output, unsigned short v);

        void writeRGB(std::ofstream &output, int v);

        void preprocessVoxels(std::vector<Voxel> &voxels);

        int parseInt(const std::string& s);
    }
}
#endif //PROJECT2_FILEMAKER_H
