//
// Created by bogum on 05.06.2022.
//

#include "TempFace.h"

TempFace* TempFace::fromFace(Face face, ObjectStore* objectStore) {
    auto* f = new TempFace();
    auto* vertices = objectStore->getVertices();
    auto* textures = objectStore->getTextures();
    TempFace::v1 = vertices[face.getVertices()[0] - 1];
    if (face.getTextures()[0] != -1) {
        TempFace::t1 = textures[face.getTextures()[0] -1];
    }
    TempFace::v2 = vertices[face.getVertices()[1] - 1];
    if (face.getTextures()[1] != -1) {
        TempFace::t2 = textures[face.getTextures()[1] -1];
    }
    TempFace::v3 = vertices[face.getVertices()[2] - 1];
    if (face.getTextures()[2] != -1) {
        TempFace::t3 = textures[face.getTextures()[2] -1];
    }
    return f;
}

const std::vector<double> &TempFace::getV1() const {
    return v1;
}

void TempFace::setV1(const std::vector<double> &set_v1) {
    TempFace::v1 = set_v1;
}

const std::vector<double> &TempFace::getT1() const {
    return t1;
}

void TempFace::setT1(const std::vector<double> &set_t1) {
    TempFace::t1 = set_t1;
}

const std::vector<double> &TempFace::getV2() const {
    return v2;
}

void TempFace::setV2(const std::vector<double> &set_v2) {
    TempFace::v2 = set_v2;
}

const std::vector<double> &TempFace::getT2() const {
    return t2;
}

void TempFace::setT2(const std::vector<double> &set_t2) {
    TempFace::t2 = set_t2;
}

const std::vector<double> &TempFace::getV3() const {
    return v3;
}

void TempFace::setV3(const std::vector<double> &v3) {
    TempFace::v3 = v3;
}

const std::vector<double> &TempFace::getT3() const {
    return t3;
}

void TempFace::setT3(const std::vector<double> &set_t3) {
    TempFace::t3 = set_t3;
}
