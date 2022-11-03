//
// Created by bogum on 30.09.2022.
//

#include "ObjectStore.h"

ObjectStore::ObjectStore() {
    ObjectStore::max_x = std::numeric_limits<float>::min();
    ObjectStore::max_y = std::numeric_limits<float>::min();
    ObjectStore::max_z = std::numeric_limits<float>::min();
    ObjectStore::min_x = std::numeric_limits<float>::max();
    ObjectStore::min_y = std::numeric_limits<float>::max();
    ObjectStore::min_z = std::numeric_limits<float>::max();
}

std::vector<double> *ObjectStore::getVertices() const {
    return vertices;
}

std::vector<double> *ObjectStore::getTextures() const {
    return textures;
}

std::vector<double> *ObjectStore::getNormals() const {
    return normals;
}

Face *ObjectStore::getFaces() const {
    return faces;
}

void ObjectStore::setVertices(std::vector<double> *set_vertices) {
    ObjectStore::vertices = set_vertices;
}

void ObjectStore::setTextures(std::vector<double> *set_textures) {
    ObjectStore::textures = set_textures;
}

void ObjectStore::setNormals(std::vector<double> *set_normals) {
    ObjectStore::normals = set_normals;
}

void ObjectStore::setFaces(Face *set_faces) {
    ObjectStore::faces = set_faces;
}

void ObjectStore::calculateDimensions() {
    for (int i = 0; i < ObjectStore::getSizeVertices(); i++) {
        if (vertices[i].front() < min_x) {
            min_x = (float) vertices[i].front();
        }
        if (vertices[i].at(1) < min_y) {
            min_x = (float) vertices[i].at(1);
        }
        if (vertices[i].back() < min_z) {
            min_x = (float) vertices[i].back();
        }
        if (vertices[i].front() > max_x) {
            max_x = (float) vertices[i].front();
        }
        if (vertices[i].at(1) > max_y) {
            max_x = (float) vertices[i].at(1);
        }
        if (vertices[i].back() > max_z) {
            max_x = (float) vertices[i].back();
        }
    }
}

int ObjectStore::getSizeFaces() const {
    return size_faces;
}

void ObjectStore::setSizeFaces(int sizeFaces) {
    size_faces = sizeFaces;
}

int ObjectStore::getSizeVertices() const {
    return size_vertices;
}

void ObjectStore::setSizeVertices(int sizeVertices) {
    size_vertices = sizeVertices;
}

int ObjectStore::getSizeTextures() const {
    return size_textures;
}

void ObjectStore::setSizeTextures(int sizeTextures) {
    size_textures = sizeTextures;
}

int ObjectStore::getSizeNormals() const {
    return size_normals;
}

void ObjectStore::setSizeNormals(int sizeNormals) {
    size_normals = sizeNormals;
}

ObjectStore::~ObjectStore() {
    delete[] vertices;
    delete[] textures;
    delete[] normals;
    delete[] faces;
}
