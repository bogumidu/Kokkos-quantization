//
// Created by bogum on 15.04.2022.
//

#include <vector>
#include <list>
#include <stdexcept>
#include "Face.h"

// could be changed into a template with variable array size
Face::Face(int n) {
    size = n;
    vertices = new int[n];
    textures = new int[n];
    normals = new int[n];
    for (int i = 0; i < n; i++) {
        vertices[i] = -1;
        textures[i] = -1;
        normals[i] = -1;
    }
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

void Face::setMaterial(const std::string& set_material) {
    Face::material = set_material;
}

void Face::setGroup(const std::string& set_group) {
    Face::group = set_group;
}

void Face::setVertex(int index, int vertex, int texture, int normal) {
    if (index >= size) throw std::runtime_error("Index out of range in face");
    vertices[index] = vertex;
    textures[index] = texture;
    normals[index] = normal;
}

std::list<Face> Face::forceTriangles() {
    if (size == 3) {
        std::list<Face> result;
        result.push_back(*this);
        return result;
    }
    std::list<Face> result;
    for (int i = 0; i < size - 2; i++) {
        Face f = Face(3);
        f.setVertex(0, vertices[0], textures[0], normals[0]);
        f.setVertex(1, vertices[1 + i], textures[1 + i], normals[1 + i]);
        f.setVertex(2, vertices[2 + i], textures[2 + i], normals[2 + i]);
        result.push_back(f);
    }
    return result;
}
