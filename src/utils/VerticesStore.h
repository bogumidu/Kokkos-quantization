//
// Created by bogum on 22.10.2022.
//

#ifndef PROJECT2_VERTICESSTORE_H
#define PROJECT2_VERTICESSTORE_H


#include <list>
#include <vector>

class VerticesStore {
private:
    std::list<std::vector<double>> vertices;

public:
    void putVertex(const std::vector<double>& v);
    std::vector<double>* listUnique();
};


#endif //PROJECT2_VERTICESSTORE_H
