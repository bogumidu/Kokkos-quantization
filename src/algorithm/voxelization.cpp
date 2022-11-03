//
// Created by bogum on 11.10.2022.
//

#include <stdexcept>
#include <valarray>
#include <cmath>
#include "voxelization.h"

// TODO: swap TempFace with Face
void voxelization::teselation::teselate(TempFace *face, int accuracy, ObjectStore *objectStore,
                                        std::list<std::vector<double>>* vertices) {
    if (accuracy <= 0) return;
    if (utils::almostEqual(face->getV1(), face->getV2()) && utils::almostEqual(face->getV2(), face->getV3())) return;

    accuracy--;
    auto v1 = face->getV1();
    auto v2 = face->getV2();
    auto v3 = face->getV3();
    auto t1 = face->getT1();
    auto t2 = face->getT2();
    auto t3 = face->getT3();

}

// TODO: swap TempFace with Face
Voxel *
voxelization::teselation::quantizationAlgorithm(TempFace *face, int accuracy, Voxel *voxel, VoxelStore *voxelStore) {
    voxel = createVoxel(face, voxel, voxelStore);
    if (accuracy <= 0) return voxel;
    if (utils::almostEqual(face->getV1(), face->getV2()) && utils::almostEqual(face->getV2(), face->getV3())) {
        return voxel;
    }
    accuracy--;
    auto v1 = face->getV1();
    auto v2 = face->getV2();
    auto v3 = face->getV3();
    auto t1 = face->getT1();
    auto t2 = face->getT2();
    auto t3 = face->getT3();

// TODO: swap TempFace with Face
    auto *f1 = new TempFace();
    auto *f2 = new TempFace();
    auto *f3 = new TempFace();
    auto *f4 = new TempFace();

    std::vector<double> v12 = {(v1.front() + v2.front()) / 2, (v1.at(1) + v2.at(1)) / 2,
                               (v1.back() + v2.back()) / 2};
    std::vector<double> v13 = {(v1.front() + v3.front()) / 2, (v1.at(1) + v3.at(1)) / 2,
                               (v1.back() + v3.back()) / 2};
    std::vector<double> v23 = {(v3.front() + v2.front()) / 2, (v3.at(1) + v2.at(1)) / 2,
                               (v3.back() + v2.back()) / 2};
    std::vector<double> t12;
    std::vector<double> t13;
    std::vector<double> t23;
    // TODO: not sure if texture is always created at object construction
    if (!face->getT1().empty()) {
        t12 = {(face->getT1().front() + face->getT2().front()) / 2,
               (face->getT1().at(1) + face->getT2().at(1)) / 2,
               (face->getT1().back() + face->getT2().back()) / 2};
        t13 = {(face->getT1().front() + face->getT3().front()) / 2,
               (face->getT1().at(1) + face->getT3().at(1)) / 2,
               (face->getT1().back() + face->getT3().back()) / 2};
        t23 = {(face->getT3().front() + face->getT2().front()) / 2,
               (face->getT3().at(1) + face->getT2().at(1)) / 2,
               (face->getT3().back() + face->getT2().back()) / 2};
    }
    // TODO: change to get created as array
    f1->setV1(v1);
    f1->setV2(v13);
    f1->setV3(v12);
    f1->setT1(t1);
    f1->setT2(t13);
    f1->setT3(t12);

    f2->setV1(v12);
    f2->setV2(v23);
    f2->setV3(v2);
    f2->setT1(t12);
    f2->setT2(t23);
    f2->setT3(t2);

    f3->setV1(v13);
    f3->setV2(v3);
    f3->setV3(v23);
    f3->setT1(t13);
    f3->setT2(t3);
    f3->setT3(t23);

    f4->setV1(v13);
    f4->setV2(v23);
    f4->setV3(v12);
    f4->setT1(t13);
    f4->setT2(t23);
    f4->setT3(t12);

    voxel = teselation::quantizationAlgorithm(f1, accuracy, voxel, voxelStore);
    voxel = teselation::quantizationAlgorithm(f2, accuracy, voxel, voxelStore);
    voxel = teselation::quantizationAlgorithm(f3, accuracy, voxel, voxelStore);
    voxel = teselation::quantizationAlgorithm(f4, accuracy, voxel, voxelStore);
    return voxel;
}

int voxelization::teselation::getTeselationLevels(Face *face, ObjectStore *objectStore) {
//    std::vector<double> v1 = objectStore->getVertices()[face->getVertices()[0] - 1];
//    std::vector<double> v2 = objectStore->getVertices()[face->getVertices()[1] - 1];
//    std::vector<double> v3 = objectStore->getVertices()[face->getVertices()[2] - 1];
//    int result = (int) round(std::log2(std::max(std::max(utils::distance(v1, v2), utils::distance(v1, v3)),
//                                                utils::distance(v2, v3))));
    return 0;
}


// TODO: swap TempFace with Face
Voxel *voxelization::voxelization::createVoxel(TempFace *face, Voxel *voxel, VoxelStore *voxelStore) {
    std::vector<double> t;
    std::vector<double> v;
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            t = face->getT1();
            v = face->getV1();
        } else if (i == 1) {
            t = face->getT2();
            v = face->getV2();
        } else {
            t = face->getT3();
            v = face->getV3();
        }
        voxel->setX((int) v.front());
        voxel->setY((int) v.at(1));
        voxel->setZ((int) v.back());
        if (voxelStore->hasVoxel(voxel)) continue;
        voxel->setColor(0xffffff);
        voxel = storeVoxel(voxel, voxelStore);
    }
    return voxel;
}

Voxel *voxelization::voxelization::storeVoxel(Voxel *voxel, VoxelStore *voxelStore) {
    if (!voxelStore->canPut()) {
        // TODO: need storage for store units
    }
    return nullptr;
}

bool voxelization::utils::almostEqual(std::vector<double> v1, std::vector<double> v2) {
    if (v1.size() != 3 || v2.size() != 3) {
        throw std::runtime_error("Cannot run almostEqual operation. Improper vector length.");
    }
    return (((int) v1.front()) == ((int) v2.front()) && ((int) v1.at(1)) == ((int) v2.at(1)) &&
            ((int) v1.back()) == ((int) v2.back()));
}

double voxelization::utils::distance(std::vector<double> v1, std::vector<double> v2) {
    double result = sqrt(pow(v1.front() - v2.front(), 2) + pow(v1.at(1) - v2.at(1), 2)
                         + pow(v1.back() - v2.back(), 2));
    return result;
}
