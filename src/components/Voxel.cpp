//
// Created by bogum on 05.06.2022.
//

#include "Voxel.h"

int Voxel::getColor() const {
    return color;
}

void Voxel::setColor(int set_color) {
    Voxel::color = set_color;
}

Voxel::~Voxel() = default;

std::string Voxel::toString() {
    return std::string("Voxel{x=") += std::to_string(getX()) += std::string(", y=")
            += std::to_string(getY()) += std::string(", z=") += std::to_string(getZ())
                    += std::string(", color=") += std::to_string(getColor()) += std::string("}");
}

int Voxel::getX() const {
    return this->x;
}

int Voxel::getY() const {
    return this->y;
}

int Voxel::getZ() const {
    return this->z;
}

void Voxel::setX(int set_x) {
    this->x = set_x;
}

void Voxel::setY(int set_y) {
    this->y = set_y;
}

void Voxel::setZ(int set_z) {
    this->z = set_z;
}

bool Voxel::operator==(const Voxel &rhs) const {
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z &&
           color == rhs.color;
}

bool Voxel::operator!=(const Voxel &rhs) const {
    return !(rhs == *this);
}

Voxel::Voxel(int x, int y, int z, int color) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->color = color;

}

Voxel::Voxel() = default;

