//
// Created by bogum on 14.04.2022.
//

#ifndef PROJECT2_FILEMAKER_H
#define PROJECT2_FILEMAKER_H

#include <list>
#include <vector>
#include "../components/Voxel.h"

namespace FileMaker {

    inline namespace testFM {
        void test();
    }

    inline namespace loadFile{
        void loadObject(const std::string& fileName);
        std::vector<Voxel> loadSchematic(const std::string& fileName);
//        void assignPropperites(float *maxX) { *maxX = maxX; }
        void saveSchematic(const std::string &fileName, const std::vector<Voxel> &voxels);
    }

    inline namespace utils {
        std::list<std::string> split(std::string &str, char delimiter);

        unsigned short readUnsignedShort(std::ifstream &input);

        int readRGB(std::ifstream &input);

        void writeUnsignedShort(std::ofstream &output, unsigned short v);

        int writeRGB(std::ofstream &output, int v);
    }
}
#endif //PROJECT2_FILEMAKER_H
