//
// Created by bogum on 05.06.2022.
//

#ifndef PROJECT2_VOXEL_H
#define PROJECT2_VOXEL_H


#include "Location.h"
#include <string>

struct Voxel : public Location {

private:
    int color;

public:
    Voxel(int x, int y, int z, int color);

    virtual ~Voxel();

    int getColor() const;

    void setColor(int color);

    std::string toString();

};


#endif //PROJECT2_VOXEL_H
