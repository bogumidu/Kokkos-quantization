//
// Created by bogum on 05.06.2022.
//

#ifndef PROJECT2_VOXEL_H
#define PROJECT2_VOXEL_H

#include <string>

struct Voxel {

private:
    int x;
    int y;
    int z;
    int color;

public:
    Voxel();

    Voxel(int x, int y, int z, int color);

     ~Voxel();

    [[nodiscard]] int getX() const;
    [[nodiscard]] int getY() const;
    [[nodiscard]] int getZ() const;
    void setX(int set_x);
    void setY(int set_y);
    void setZ(int set_z);

    [[nodiscard]] int getColor() const;

    void setColor(int set_color);

    std::string toString();

    bool operator==(const Voxel &rhs) const;

    bool operator!=(const Voxel &rhs) const;

    bool operator<(const Voxel &rhs) const;

    bool operator>(const Voxel &rhs) const;

    bool operator<=(const Voxel &rhs) const;

    bool operator>=(const Voxel &rhs) const;

};


#endif //PROJECT2_VOXEL_H
