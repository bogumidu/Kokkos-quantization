//
// Created by bogum on 22.10.2022.
//

#include "VerticesStore.h"

void VerticesStore::putVertex(const std::vector<double>& v) {
    VerticesStore::vertices.push_back(v);
}

std::vector<double> *VerticesStore::listUnique() {
    auto vertices_ = VerticesStore::vertices;
    vertices_.unique();
    std::vector<double>
    return nullptr;
}
