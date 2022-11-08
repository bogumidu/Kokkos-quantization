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

Voxel::Voxel(int x, int y, int z, int color) : Location(x, y, z), color(color) {}

Voxel::~Voxel() = default;

std::string Voxel::toString() {
    return std::string("Voxel{x=") += std::to_string(getX()) += std::string(", y=")
            += std::to_string(getY()) += std::string(", z=") += std::to_string(getZ())
                    += std::string(", color=") += std::to_string(getColor()) += std::string("}");
}

