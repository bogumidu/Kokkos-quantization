//
// Created by bogum on 05.06.2022.
//

#include "VoxelStore.h"
#include <stdexcept>
#include <iostream>

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
    if (!hasVoxel(put_v)) {
        voxels_deque.push_back(put_v);
    }
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
    int deque_size = (int) voxels_deque.size() - 1;
    Voxel voxels_array[deque_size];
    for (int i = 0; i < deque_size; i++) {
        voxels_array[i] = voxels_deque.at(i);
    }
    this->setVoxels(voxels_array);
    return this->voxels;
}
