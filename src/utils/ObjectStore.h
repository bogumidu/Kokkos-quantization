//
// Created by bogum on 30.09.2022.
//

#ifndef PROJECT2_OBJECTSTORE_H
#define PROJECT2_OBJECTSTORE_H


#include <vector>
#include "Face.h"

struct ObjectStore {
private:
    int size_faces;

    int size_vertices;

    int size_textures;

    int size_normals;

    float max_x;

    float max_y;

    float max_z;

    float min_x;

    float min_y;

    float min_z;

    std::vector<double> *vertices{};

    std::vector<double> *textures{};

    std::vector<double> *normals{};

    Face *faces{};

public:
    [[nodiscard]] int getSizeFaces() const;

    void setSizeFaces(int sizeFaces);

    [[nodiscard]] int getSizeVertices() const;

    void setSizeVertices(int sizeVertices);

    [[nodiscard]] int getSizeTextures() const;

    void setSizeTextures(int sizeTextures);

    [[nodiscard]] int getSizeNormals() const;

    void setSizeNormals(int sizeNormals);

    explicit ObjectStore();

    virtual ~ObjectStore();

    [[nodiscard]] std::vector<double> *getVertices() const;

    [[nodiscard]] std::vector<double> *getTextures() const;

    [[nodiscard]] std::vector<double> *getNormals() const;

    [[nodiscard]] Face *getFaces() const;

    void setVertices(std::vector<double> *set_vertices);

    void setTextures(std::vector<double> *set_textures);

    void setNormals(std::vector<double> *set_normals);

    void setFaces(Face *set_faces);

    void calculateDimensions();

};


#endif //PROJECT2_OBJECTSTORE_H
