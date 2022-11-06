//
// Created by bogum on 11.10.2022.
//

#include <stdexcept>
#include <valarray>
#include <cmath>
#include <vector>
#include "algorithm.h"
#include "../components/Face.h"

// recursion function tessellation + algorithm
void algorithm::quantizationAlgorithm(Face *faces, int accuracy, int index, VoxelStore *voxelStore) {
    Face face = faces[index];
    auto vertices = face.getVertices();
    auto textures = face.getTextures();
    // exit state, voxel creation
    if (accuracy <= 0 || (utils::almostEqual(vertices[0], vertices[1]) && utils::almostEqual(vertices[1], vertices[2]))) {
        int color = 0xffffff;
        // TODO: skipped material colorAt() function
        auto* voxel1 = new Voxel((int) vertices[0].front(), (int) vertices[0].at(1), (int) vertices[0].back(), color);
        auto* voxel2 = new Voxel((int) vertices[1].front(), (int) vertices[1].at(1), (int) vertices[1].back(), color);
        auto* voxel3 = new Voxel((int) vertices[2].front(), (int) vertices[2].at(1), (int) vertices[2].back(), color);
        voxelStore->putVoxel(voxel1);
        voxelStore->putVoxel(voxel2);
        voxelStore->putVoxel(voxel3);
        return;
    }
    // tessellation and recursion
    accuracy--;
    std::vector<double> v12 = {(vertices[0].front() + vertices[1].front()) / 2, (vertices[0].at(1) + vertices[1].at(1)) / 2,
                               (vertices[0].back() + vertices[1].back()) / 2};
    std::vector<double> v13 = {(vertices[0].front() + vertices[2].front()) / 2, (vertices[0].at(1) + vertices[2].at(1)) / 2,
                               (vertices[0].back() + vertices[2].back()) / 2};
    std::vector<double> v23 = {(vertices[2].front() + vertices[1].front()) / 2, (vertices[2].at(1) + vertices[1].at(1)) / 2,
                               (vertices[2].back() + vertices[1].back()) / 2};
    std::vector<double> t12;
    std::vector<double> t13;
    std::vector<double> t23;
    if (!textures->empty()) {
        t12 = {(textures[0].front() + textures[1].front()) / 2,
               (textures[0].back() + textures[1].back()) / 2};
        t13 = {(textures[0].front() + textures[2].front()) / 2,
               (textures[0].back() + textures[2].back()) / 2};
        t23 = {(textures[2].front() + textures[1].front()) / 2,
               (textures[2].back() + textures[1].back()) / 2};
    }
    Face f1;
    Face f2;
    Face f3;
    Face f4;

    std::vector<double> f1v[3] = {vertices[0], v13, v12};
    std::vector<double> f2v[3] = {v12, v23, vertices[1]};
    std::vector<double> f3v[3] = {v13, vertices[2], v23};
    std::vector<double> f4v[3] = {v13, v23, v12};

    std::vector<double> f1t[3] = {textures[0], t13, t12};
    std::vector<double> f2t[3] = {t12, t23, textures[1]};
    std::vector<double> f3t[3] = {t13, textures[2], t23};
    std::vector<double> f4t[3] = {t13, t23, t12};

    f1.setVertices(f1v);
    f2.setVertices(f2v);
    f3.setVertices(f3v);
    f4.setVertices(f4v);

    f1.setTextures(f1t);
    f2.setTextures(f2t);
    f3.setTextures(f3t);
    f4.setTextures(f4t);

    Face new_faces[4] = {f1, f2, f3, f4};
    // TODO: not sure if this should create 4 functor instances or do it in a loop
    for (int i = 0; i < 4; i++) {
        quantizationAlgorithm(new_faces, accuracy, i, voxelStore);
    }
}

int algorithm::tessellation::getTessellationLevels(Face face) {
    auto vertices = face.getVertices();
    std::vector<double> v1 = vertices[0];
    std::vector<double> v2 = vertices[1];
    std::vector<double> v3 = vertices[2];
    return (int) round(std::log2(std::max(std::max(utils::distance(v1, v2), utils::distance(v1, v3)),
                                                utils::distance(v2, v3))));
}


bool algorithm::utils::almostEqual(std::vector<double> v1, std::vector<double> v2) {
    if (v1.size() != 3 || v2.size() != 3) {
        throw std::runtime_error("Cannot run almostEqual operation. Improper vector length.");
    }
    return (((int) v1.front()) == ((int) v2.front()) && ((int) v1.at(1)) == ((int) v2.at(1)) &&
            ((int) v1.back()) == ((int) v2.back()));
}


double algorithm::utils::distance(std::vector<double> v1, std::vector<double> v2) {
    double result = sqrt(pow(v1.front() - v2.front(), 2) + pow(v1.at(1) - v2.at(1), 2)
                         + pow(v1.back() - v2.back(), 2));
    return result;
}
