//
// Created by bogum on 05.06.2022.
//

#ifndef PROJECT2_VOXELSTORE_H
#define PROJECT2_VOXELSTORE_H


#include <deque>
#include "../components/Voxel.h"

struct VoxelStore {

private:
    int size;
    std::deque<Voxel> voxels_deque;
    Voxel *voxels{};

public:
    explicit VoxelStore();

    virtual ~VoxelStore();

    [[nodiscard]] int getSize() const;

    void setSize(int set_size);

    [[nodiscard]] const std::deque<Voxel> &getVoxelsDeque() const;

    void setVoxelsDeque(const std::deque<Voxel> &voxelsDeque);

    [[nodiscard]] Voxel *getVoxels() const;

    void setVoxels(Voxel *set_voxels);

    void putVoxel(const Voxel& put_v);

    bool hasVoxel(const Voxel& temp_v);

    Voxel* convertToArray();

};


#endif //PROJECT2_VOXELSTORE_H
