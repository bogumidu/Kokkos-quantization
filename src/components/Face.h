//
// Created by bogum on 15.04.2022.
//

#ifndef PROJECT2_FACE_H
#define PROJECT2_FACE_H


#include <string>
#include <list>

struct Face {

private:
    int size;

    // x, y, z vector
    std::vector<double> *vertices{};

    // u, v vector
    std::vector<double> *textures{};

    // x, y, z vector
    std::vector<double> *normals{};

    // material name
    std::string material;

    // group name
    std::string group;

public:
    explicit Face(int n = 3);

    virtual ~Face();

    [[nodiscard]] std::vector<double> *getVertices();

    [[nodiscard]] std::vector<double> *getTextures();

    [[nodiscard]] std::vector<double> *getNormals();

    [[nodiscard]] const std::string &getMaterial();

    [[nodiscard]] const std::string &getGroup();

    void setMaterial(const std::string& set_material);

    void setGroup(const std::string& set_group);

    void setVertex(int index, std::vector<double> vertex, std::vector<double> texture, std::vector<double> normal);

    void setVertices(std::vector<double> *vertices);

    void setTextures(std::vector<double> *textures);

    void setNormals(std::vector<double> *normals);

    std::list<Face> forceTriangles();

};


#endif //PROJECT2_FACE_H
