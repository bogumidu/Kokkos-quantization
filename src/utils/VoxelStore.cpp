//
// Created by bogum on 05.06.2022.
//

#include "VoxelStore.h"
#include <stdexcept>
#include <iostream>

VoxelStore::VoxelStore(int size) : size(size) {
    this->size = size;
    bytes = new unsigned char[size];
}

VoxelStore::~VoxelStore() = default;

int VoxelStore::getSize() const {
    return size;
}

void VoxelStore::setSize(int size) {
    VoxelStore::size = size;
}

void VoxelStore::putVoxel(Voxel v) {
    try {
        putInt(v.getX());
        putInt(v.getY());
        putInt(v.getZ());
        blockCount++;
    } catch (std::invalid_argument& ex) {
        std::cout << "End of file reached: \n" << ex.what() << "\n";
    }
}

Voxel VoxelStore::readVoxel() {
    try {
        Voxel v = Voxel(readInt(), readInt(), readInt(), readInt());
        return v;
    } catch (std::invalid_argument& ex) {
        std::cout << "End of file reached: \n" << ex.what() << "\n";
    }
}

void VoxelStore::putInt(int v) {
    write((v >> 24) & 0xFF);
    write((v >> 16) & 0xFF);
    write((v >> 8) & 0xFF);
    write((v >> 0) & 0xFF);
}

int VoxelStore::readInt() {
    int ch1 = read();
    int ch2 = read();
    int ch3 = read();
    int ch4 = read();
    if ((ch1 | ch2 | ch3 | ch4) < 0) {
        throw std::invalid_argument("");
    }
    return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
}

int VoxelStore::readIntAt(int pos) {
    int ch1 = readAt(pos);
    int ch2 = readAt(pos + 1);
    int ch3 = readAt(pos + 2);
    int ch4 = readAt(pos + 3);
    if ((ch1 | ch2 | ch3 | ch4) < 0) {
        throw std::invalid_argument("");
    }
    return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
}

void VoxelStore::write(int i) {
    if (position >= getSize()) throw std::invalid_argument("");
    bytes[position] = (unsigned char) i;
    position++;
}

int VoxelStore::read() {
    return (position < getSize()) ? (bytes[position++] & 0xFF) : -1;
}

int VoxelStore::readAt(int pos) {
    return (position < getSize()) ? (bytes[pos] & 0xff) : -1;
}

bool VoxelStore::hasNext() {
    return blockCount > 0;
}

bool VoxelStore::canPut() {
    return getSize() - position >= 16;
}

void VoxelStore::resetPosition() {
    this->position = 0;
}

bool VoxelStore::hasVoxel(Voxel v) {
    for (int i = position - 16; i >= 0; i-= 16) {
        try {
            if (v.getX() == readIntAt(i) && v.getY() == readIntAt(i + 4) && v.getZ() == readIntAt(i + 8)) {
                return true;
            }
        } catch (std::invalid_argument& ex) {
            std::cout << "End of file reached: \n" << ex.what() << "\n";
        }
    }
    return false;
}
