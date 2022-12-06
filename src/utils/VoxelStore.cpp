//
// Created by bogum on 05.06.2022.
//

#include "VoxelStore.h"
#include <stdexcept>
#include <iostream>
#include <map>

VoxelStore::VoxelStore() = default;

VoxelStore::~VoxelStore() = default;

int VoxelStore::getSize() const {
    return size;
}

void VoxelStore::setSize(int set_size) {
    VoxelStore::size = set_size;
}

const std::deque<Voxel> &VoxelStore::getVoxelsDeque() const {
    return voxels_deque;
}

void VoxelStore::setVoxelsDeque(const std::deque<Voxel> &voxelsDeque) {
    voxels_deque = voxelsDeque;
}

Voxel *VoxelStore::getVoxels() const {
    return voxels;
}

void VoxelStore::setVoxels(Voxel *set_voxels) {
    VoxelStore::voxels = set_voxels;
}

void VoxelStore::putVoxel(const Voxel& put_v) {
//    if (!hasVoxel(put_v)) {
        voxels_deque.push_back(put_v);
//    }
}

bool VoxelStore::hasVoxel(const Voxel& temp_v) {
    auto deque_index = voxels_deque.size() - 1;
    for (int i = (int) deque_index; i >= 0; i--) {
        if (temp_v == voxels_deque.at(i)) {
            return true;
        }
    }
    return false;
}

Voxel *VoxelStore::convertToArray() {
    auto vmap = std::map<int, std::map<int, std::map<int, int>>>();
    int deque_size = (int) voxels_deque.size();
    auto* voxels_array = new Voxel[deque_size];
    std::cout << "Deque size: " << deque_size << std::endl;
    int temp_size = 0;
    for (int i = 0; i < deque_size; i++) {
        auto temp_v = voxels_deque.at(i);
        if (vmap.find(temp_v.getX()) == vmap.end()) {
            vmap[temp_v.getX()] = std::map<int, std::map<int, int>>();
        }
        if (vmap[temp_v.getX()].find(temp_v.getY()) == vmap[temp_v.getX()].end()) {
            vmap[temp_v.getX()][temp_v.getY()] = std::map<int, int>();
        }
        if (vmap[temp_v.getX()][temp_v.getY()].find(temp_v.getZ()) == vmap[temp_v.getX()][temp_v.getY()].end()) {
            vmap[temp_v.getX()][temp_v.getY()][temp_v.getZ()] = temp_v.getColor();
        } else {
            continue;
        }
        temp_size++;
        voxels_array[i] = voxels_deque.at(i);
    }
    std::cout << "Size: " << temp_size << std::endl;
    this->setVoxels(voxels_array);
    this->setSize(temp_size);
    return this->voxels;
}
