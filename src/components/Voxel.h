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
    Voxel();

    Voxel(int x, int y, int z, int color);

    virtual ~Voxel();

    [[nodiscard]] int getColor() const;

    void setColor(int set_color);

    std::string toString();

    bool operator==(const Voxel &rhs) const;

    bool operator!=(const Voxel &rhs) const;

};


#endif //PROJECT2_VOXEL_H
