//
// Created by bogum on 15.04.2022.
//

#include <vector>
#include "Face.h"

// could be changed into a template with variable array size
Face::Face(int n) {
    size = n;
    vertices = new int[n];
    textures = new int[n];
    normals = new int[n];
}

Face::~Face() = default;

int * Face::getVertices() {
    return vertices;
}

int * Face::getTextures() {
    return textures;
}

int * Face::getNormals() {
    return normals;
}

const std::string &Face::getMaterial() {
    return material;
}

const std::string &Face::getGroup() {
    return group;
}

void Face::setMaterial(const std::string material) {
    Face::material = material;
}

void Face::setGroup(const std::string group) {
    Face::group = group;
}
