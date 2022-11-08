//
// Created by bogum on 08.11.2022.
//

#include "Location.h"
#include <cmath>


Location::Location(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

int Location::getX() const {
    return this->x;
}

int Location::getY() const {
    return this->y;
}

int Location::getZ() const {
    return this->z;
}

int Location::getLocalId() const {
    return (abs((this->x) % 16 << 10) + (abs(this->y % 16) << 5) + abs(this->z % 16));
}

void Location::setX(int set_x) {
    this->x = set_x;
}

void Location::setY(int set_y) {
    this->y = set_y;
}

void Location::setZ(int set_z) {
    this->z = set_z;
}

void Location::setLocation(int set_x, int set_y, int set_z) {
    this->x = set_x;
    this->y = set_y;
    this->z = set_z;
}

bool Location::operator==(const Location &rhs) const {
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z;
}

bool Location::operator!=(const Location &rhs) const {
    return !(rhs == *this);
}

Location::Location() = default;
