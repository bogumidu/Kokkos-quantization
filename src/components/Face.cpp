//
// Created by bogum on 15.04.2022.
//

#include <utility>
#include <vector>
#include <list>
#include <stdexcept>
#include "Face.h"

// could be changed into a template with variable array size
Face::Face(int n) {
    size = n;
    vertices = new std::vector<double>[n];
    textures = new std::vector<double>[n];
    normals = new std::vector<double>[n];
    for (int i = 0; i < n; i++) {
        vertices[i] = {-1, -1, -1};
        textures[i] = {-1, -1};
        normals[i] = {-1, -1, -1};
    }
}

Face::~Face() = default;

std::vector<double> * Face::getVertices() {
    return vertices;
}

std::vector<double> * Face::getTextures() {
    return textures;
}

std::vector<double> * Face::getNormals() {
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

void Face::setVertex(int index, std::vector<double> vertex, std::vector<double> texture, std::vector<double> normal) {
    if (index >= size) throw std::runtime_error("Index out of range in face");
    vertices[index] = std::move(vertex);
    textures[index] = std::move(texture);
    normals[index] = std::move(normal);
}

void Face::setLine(int l) {
    line = l;
}

std::list<Face> Face::forceTriangles() {
    if (size == 3) {
        std::list<Face> result;
        Face f = Face(3);
        f.setLine(line);
        f.setVertex(0, vertices[0], textures[0], normals[0]);
        f.setVertex(1, vertices[1], textures[1], normals[1]);
        f.setVertex(2, vertices[2], textures[2], normals[2]);
        result.push_back(f);
        return result;
    }
    std::list<Face> result;
    for (int i = 0; i < size - 2; i++) {
        Face f = Face(3);
        f.setLine(line);
        f.setVertex(0, vertices[0], textures[0], normals[0]);
        f.setVertex(1, vertices[1 + i], textures[1 + i], normals[1 + i]);
        f.setVertex(2, vertices[2 + i], textures[2 + i], normals[2 + i]);
        result.push_back(f);
    }
    return result;
}

void Face::setVertices(std::vector<double> *vertices) {
    Face::vertices = vertices;
}

void Face::setTextures(std::vector<double> *textures) {
    Face::textures = textures;
}

void Face::setNormals(std::vector<double> *normals) {
    Face::normals = normals;
}

std::ostream &operator<<(std::ostream &os, const Face &face) {
    os << "line: " << face.line << ", size: " << face.size << " vertices: {["
    << face.vertices[0].front() << ", " << face.vertices[0].at(1) << ", " << face.vertices[0].back() << "], ["
    << face.vertices[1].front() << ", " << face.vertices[1].at(1) << ", " << face.vertices[1].back() << "], ["
    << face.vertices[2].front() << ", " << face.vertices[2].at(1) << ", " << face.vertices[2].back() << "]} "
    << " textures: " << face.textures << " normals: " << face.normals
    << " material: " << face.material << " group: " << face.group;
    return os;
}
