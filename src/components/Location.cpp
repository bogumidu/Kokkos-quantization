//
// Created by bogum on 10.04.2022.
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

int Location::getLocalId() {
    return (abs((this->x) % 16 << 10) + (abs(this->y % 16) << 5) + abs(this->z % 16));
}

void Location::setX(int x) {
    this->x = x;
}

void Location::setY(int y) {
    this->y = y;
}

void Location::setZ(int z) {
    this->z = z;
}

void Location::setLocation(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}