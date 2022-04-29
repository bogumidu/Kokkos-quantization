//
// Created by bogum on 15.04.2022.
//

#ifndef PROJECT2_FACE_H
#define PROJECT2_FACE_H


#include <string>

struct Face {

private:
    int size;

    int *vertices;

    int *textures;

    int *normals;

    std::string material;

    std::string group;

public:
    explicit Face(int n);

    virtual ~Face();

    [[nodiscard]] int *getVertices();

    [[nodiscard]] int *getTextures();

    [[nodiscard]] int *getNormals();

    [[nodiscard]] const std::string &getMaterial();

    [[nodiscard]] const std::string &getGroup();

    void setMaterial(std::string material);

    void setGroup(std::string group);

};


#endif //PROJECT2_FACE_H
