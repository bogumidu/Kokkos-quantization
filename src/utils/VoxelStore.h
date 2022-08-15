//
// Created by bogum on 05.06.2022.
//

#ifndef PROJECT2_VOXELSTORE_H
#define PROJECT2_VOXELSTORE_H


#include "../components/Voxel.h"

struct VoxelStore {

private:
    int size;
    unsigned char *bytes;
    int position = 0;
    int blockCount = 0;

public:
    explicit VoxelStore(int size);

    virtual ~VoxelStore();

    [[nodiscard]] int getSize() const;

    void setSize(int size);

    void putVoxel(Voxel v);

    Voxel readVoxel();

    void putInt(int v);

    int readInt();

    int readIntAt(int pos);

    void write(int i);

    int read();

    int readAt(int pos);

    bool hasNext();

    bool canPut();

    void resetPosition();

    bool hasVoxel(Voxel v);

};


#endif //PROJECT2_VOXELSTORE_H
