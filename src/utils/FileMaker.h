//
// Created by bogum on 14.04.2022.
//

#ifndef PROJECT2_FILEMAKER_H
#define PROJECT2_FILEMAKER_H

#include <list>

namespace FileMaker {

    inline namespace testFM {
        void test();
    }

    inline namespace loadFile{
        void loadObject(std::string fileName);
        void loadSchematic(std::string fileName);
//        void assignPropperites(float *maxX) { *maxX = maxX; }
    }

    inline namespace utils {
        std::list<std::string> split(std::string &str, char delimiter);
    }
}
#endif //PROJECT2_FILEMAKER_H
