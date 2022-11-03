//
// Created by bogum on 14.04.2022.
//

#include <iostream>
#include "fileMaker.h"
#include "../components/Face.h"
#include "../components/Voxel.h"
#include "ObjectStore.h"
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <array>
#include <climits>
#include <algorithm>
#include <set>
#include <deque>

template<typename T>
T swap_endian(T u) {
    static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

    union {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}


void fileMaker::testFM::test() {
//    fileMaker::loadObject("test.obj");

}

void fileMaker::loadFile::loadObject(const std::string &fileName, ObjectStore *objectStore) {
    // stored as vector to gain access to random alocated
    // vectors are probably too large to store
    std::deque<std::vector<double>> vertices;
    std::deque<std::vector<double>> textures;
    std::deque<std::vector<double>> normals;
    std::string *material = nullptr;
    std::string *group = nullptr;
    std::string line;
    std::deque<Face> faces;
    std::fstream file("../model_files/" + fileName);
    if (file.is_open()) {
        int current_line = 0;
        while (getline(file, line)) {
            current_line++;
            if (line.empty() || line[0] == '#') continue;
            std::list<std::string> cmd = fileMaker::utils::split(line, ' ');
            if (cmd.front() == "v" || cmd.front() == "V") {
                if (cmd.size() != 4) throw std::runtime_error("Command 'v' should have 3 arguments at line");
                try {
                    double z = std::stod(cmd.back());
                    cmd.pop_back();
                    double y = std::stod(cmd.back());
                    cmd.pop_back();
                    double x = std::stod(cmd.back());
                    std::vector<double> vertices_temp = {x, y, z};
                    vertices.push_back(vertices_temp);
                } catch (std::invalid_argument &e) {
                    throw std::runtime_error("Invalid argument in command 'v': '" + line + "'");
                }
            } else if (cmd.front() == "vt" || cmd.front() == "Vt" || cmd.front() == "vT" || cmd.front() == "VT") {
                if (cmd.size() < 3) throw std::runtime_error("Command 'vt' should have at least 2 arguments");
                try {
                    double y = std::stod(cmd.back());
                    cmd.pop_back();
                    double x = std::stod(cmd.back());
                    std::vector<double> textures_temp = {x, y};
                    textures.push_back(textures_temp);
                } catch (std::invalid_argument &e) {
                    throw std::runtime_error("Invalid argument in command 'vt': '" + line + "'");
                }
            } else if (cmd.front() == "vn" || cmd.front() == "Vn" || cmd.front() == "vN" || cmd.front() == "VN") {
                if (cmd.size() != 4) throw std::runtime_error("Command 'vn' should have 3 arguments");
                try {
                    double z = std::stod(cmd.back());
                    cmd.pop_back();
                    double y = std::stod(cmd.back());
                    cmd.pop_back();
                    double x = std::stod(cmd.back());
                    std::vector<double> normals_temp = {x, y, z};
                    normals.push_back(normals_temp);
                } catch (std::invalid_argument &e) {
                    throw std::runtime_error("Invalid argument in command 'vn': '" + line + "'");
                }
            } else if (cmd.front() == "g" || cmd.front() == "G") {
                group = &cmd.back();
            } else if (cmd.front() == "usemtl") {
                material = &cmd.back();
            } else if (cmd.front() == "f" || cmd.front() == "F") {
                if (cmd.size() < 4) throw std::runtime_error("Command 'f' should have at least 3 arguments");
                Face temp_face = Face((int) (cmd.size() - 1));
                temp_face.setGroup(*group);
                temp_face.setMaterial(*material);
                std::vector<double> temp_vertices[cmd.size()];
                std::vector<double> temp_textures[cmd.size()];
                std::vector<double> temp_normals[cmd.size()];
                for (int i = 0; i < cmd.size(); i++) {
                    std::list<std::string> indices = fileMaker::utils::split(cmd.back(), '/');
                    temp_vertices[i] = (vertices.at(fileMaker::parseInt(indices.front()) - 1));
                    temp_textures[i] = {-1, -1};
                    temp_normals[i] = {-1, -1, -1};
                    indices.pop_front();
                    if (!indices.empty()) {
                        temp_textures[i] = textures.at(parseInt(indices.front()) - 1);
                        indices.pop_front();
                        if (!indices.empty()) temp_normals[i] = normals.at(parseInt(indices.front()) - 1);
                    }
                }
                std::list<Face> faces_triangles = temp_face.forceTriangles();
                // TODO: not sure if it is best method
                for (; !faces_triangles.empty(); faces_triangles.pop_front()) {
                    faces.push_back(faces_triangles.front());
                }
            }
        }
        Face faces_final[faces.size()];
        std::vector<double> vertices_final[vertices.size()];
        std::vector<double> textures_final[textures.size()];
        std::vector<double> normals_final[normals.size()];
        for (int i = 0; i < faces.size(); i++) {
            faces_final[i] = faces.at(i);
        }
        for (int i = 0; i < vertices.size(); i++) {
            vertices_final[i] = vertices.at(i);
        }
        for (int i = 0; i < textures.size(); i++) {
            textures_final[i] = textures.at(i);
        }
        for (int i = 0; i < normals.size(); i++) {
            normals_final[i] = normals.at(i);
        }
        objectStore->setFaces(faces_final);
        objectStore->setSizeFaces((int) faces.size());
        objectStore->setVertices(vertices_final);
        objectStore->setSizeVertices((int) vertices.size());
        objectStore->setTextures(textures_final);
        objectStore->setSizeTextures((int) textures.size());
        objectStore->setNormals(normals_final);
        objectStore->setSizeNormals((int) normals.size());
        objectStore->calculateDimensions();
        file.close();
//        std::cout << faces.size() << " " << vertices.size() << "\n" << objectStore->getSizeVertices() << "\n";
    } else std::cout << "Unable to open file";
}

std::vector<Voxel> fileMaker::loadFile::loadSchematic(const std::string &fileName) {
    std::vector<Voxel> voxels;
    std::ifstream input("../schematics/" + fileName, std::ios::in | std::ios::binary);
    if (input.is_open()) {
        auto magic = fileMaker::utils::readUnsignedShort(input);
        if (magic != 0xFAAB) throw std::runtime_error("Invalid schematic magic");
        auto paletteSize = fileMaker::utils::readUnsignedShort(input);
        std::vector<int> palette;
        for (int i = 0; i < paletteSize; i++) {
            palette.push_back(fileMaker::utils::readRGB(input));
//            std::cout << std::hex << std::uppercase << palette.back() << std::endl;
        }
        while (true) {
            int color;
            int x = fileMaker::utils::readUnsignedShort(input);
            if (input.eof()) break;
            int y = fileMaker::utils::readUnsignedShort(input);
            int z = fileMaker::utils::readUnsignedShort(input);
            if (!palette.empty()) {
                color = palette[fileMaker::utils::readUnsignedShort(input)];
            } else {
                color = fileMaker::utils::readRGB(input);
            }
            voxels.emplace_back(x, y, z, color);
        }
    }
    return voxels;
}

void fileMaker::loadFile::saveSchematic(const std::string &fileName, std::vector<Voxel> &voxels) {
    std::ofstream output("../schematics/" + fileName, std::ios::out | std::ios::binary);
    if (output.is_open()) {
        fileMaker::utils::preprocessVoxels(voxels);
        std::vector<int> palette;
        fileMaker::utils::writeUnsignedShort(output, 0xFAAB);
        fileMaker::utils::writeUnsignedShort(output, (unsigned short) palette.size());
        if (!palette.empty()) {
            for (int paletteValue: palette) {
                fileMaker::utils::writeRGB(output, paletteValue);
            }
        }
        for (const auto &voxel: voxels) {
            fileMaker::utils::writeUnsignedShort(output, voxel.getX());
            fileMaker::utils::writeUnsignedShort(output, voxel.getY());
            fileMaker::utils::writeUnsignedShort(output, voxel.getZ());
            if (!palette.empty()) {
                fileMaker::utils::writeUnsignedShort(output, (unsigned short) (
                        find(palette.begin(), palette.end(), voxel.getColor()) - palette.begin()));
            } else {
                fileMaker::utils::writeRGB(output, voxel.getColor());
            }
        }
        output.close();
    }
}

std::list<std::string> fileMaker::utils::split(std::string &str, char delimiter) {
    std::list<std::string> result;
    int start = 0;
    for (int j = 0; j < str.length(); j++) {
        if (str[j] == delimiter) {
            result.push_back(str.substr(start, j));
            start = j + 1;
        }
    }
    if (start < str.length()) {
        result.push_back(str.substr(start, str.length()));
    }
    return result;
}

unsigned short fileMaker::utils::readUnsignedShort(std::ifstream &input) {
    unsigned short v;
    input.read(reinterpret_cast<char *>(&v), sizeof(v));
    v = swap_endian(v);
    return v;
}

void fileMaker::utils::writeUnsignedShort(std::ofstream &output, unsigned short v) {
    v = swap_endian(v);
    output.write(reinterpret_cast<char *>(&v), sizeof(v));
}

int fileMaker::utils::readRGB(std::ifstream &input) {
    int v;
    input.read(reinterpret_cast<char *>(&v), 3);
    v = (v & 0xFF) << 16 | (v & 0xFF00) | (v & 0xFF0000) >> 16;
    return v;
}

void fileMaker::utils::writeRGB(std::ofstream &output, int v) {
    v = (v & 0xFF) << 16 | (v & 0xFF00) | (v & 0xFF0000) >> 16;
    output.write(reinterpret_cast<char *>(&v), 3);
}

void fileMaker::utils::preprocessVoxels(std::vector<Voxel> &voxels) {
    std::set<int> colors;
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int min_z = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    int max_z = INT_MIN;
    // Sets base coordinates and sets base points
    for (const Voxel &voxel: voxels) {
        if (voxel.getX() > max_x) max_x = voxel.getX();
        if (voxel.getY() > max_y) max_y = voxel.getY();
        if (voxel.getZ() > max_z) max_z = voxel.getZ();
        if (voxel.getX() < min_x) min_x = voxel.getX();
        if (voxel.getY() < min_y) min_y = voxel.getY();
        if (voxel.getZ() < min_z) min_z = voxel.getZ();
        colors.insert(voxel.getColor());
    }
    // Changes current coordinates to start at base point (0, 0, 0)
    for (Voxel voxel: voxels) {
        voxel.setX(voxel.getX() - min_x);
        voxel.setY(voxel.getY() - min_y);
        voxel.setZ(voxel.getZ() - min_z);
    }
    if (colors.size() < 0xFF) {
        auto paletteSize = colors.size();
        auto palette = std::vector<int>(paletteSize);
        for (int color: colors) {
            palette.push_back(color);
        }
    } else {
        auto paletteSize = 0;
    }
}

int fileMaker::utils::parseInt(const std::string &s) {
    if (s.empty()) return -1;
    return std::stoi(s);
}