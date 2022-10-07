//
// Created by bogum on 15.04.2022.
//

#ifndef PROJECT2_FACE_H
#define PROJECT2_FACE_H


#include <string>
#include <list>

struct Face {

private:
    int size{};

    int *vertices{};

    int *textures{};

    int *normals{};

    std::string material;

    std::string group;

public:
    explicit Face(int n = 3);

    virtual ~Face();

    [[nodiscard]] int *getVertices();

    [[nodiscard]] int *getTextures();

    [[nodiscard]] int *getNormals();

    [[nodiscard]] const std::string &getMaterial();

    [[nodiscard]] const std::string &getGroup();

    void setMaterial(const std::string& set_material);

    void setGroup(const std::string& set_group);

    void setVertex(int index, int vertex, int texture, int normal);

    std::list<Face> forceTriangles();

};


#endif //PROJECT2_FACE_H
