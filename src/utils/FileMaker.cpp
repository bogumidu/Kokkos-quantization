//
// Created by bogum on 14.04.2022.
//

#include <iostream>
#include "FileMaker.h"
#include "Face.h"
#include "../components/Voxel.h"
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <array>
#include <climits>
#include <algorithm>
#include <set>

template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}


void FileMaker::testFM::test() {
    FileMaker::loadObject("test.obj");

}

void FileMaker::loadFile::loadObject(const std::string& fileName) {
    std::list<double> vertices;
    std::list<double> textures;
    std::list<double> normals;
    std::string* material = nullptr;
    std::string* group = nullptr;
    std::string line;
    std::fstream file ("../model_files/" + fileName);
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::list<std::string> cmd = FileMaker::utils::split(line, ' ');
            if (cmd.front() == "v" || cmd.front() == "V") {
                if (cmd.size() != 4) throw std::runtime_error("Command 'v' should have 3 arguments");
                try {
                    double a = std::stod(cmd.back());
                    cmd.pop_back();
                    double b = std::stod(cmd.back());
                    cmd.pop_back();
                    double c = std::stod(cmd.back());
                    vertices.push_back(a);
                    vertices.push_back(b);
                    vertices.push_back(c);
                } catch (std::invalid_argument &e) {
                    throw std::runtime_error("Invalid argument in command 'v': '" + line + "'");
                }
            } else if (cmd.front() == "vt" || cmd.front() == "Vt" || cmd.front() == "vT" || cmd.front() == "VT") {
                if (cmd.size() < 3) throw std::runtime_error("Command 'vt' should have at least 2 arguments");
                try {
                    double a = std::stod(cmd.back());
                    cmd.pop_back();
                    double b = std::stod(cmd.back());
                    cmd.pop_back();
                    textures.push_back(a);
                    textures.push_back(b);
                } catch (std::invalid_argument &e) {
                    throw std::runtime_error("Invalid argument in command 'vt': '" + line + "'");
                }
            } else if (cmd.front() == "vn" || cmd.front() == "Vn" || cmd.front() == "vN" || cmd.front() == "VN") {
                if (cmd.size() != 4) throw std::runtime_error("Command 'vn' should have 3 arguments");
                try {
                    double a = std::stod(cmd.back());
                    cmd.pop_back();
                    double b = std::stod(cmd.back());
                    cmd.pop_back();
                    double c = std::stod(cmd.back());
                    normals.push_back(a);
                    normals.push_back(b);
                    normals.push_back(c);
                } catch (std::invalid_argument &e) {
                    throw std::runtime_error("Invalid argument in command 'vn': '" + line + "'");
                }
            } else if (cmd.front() == "g" || cmd.front() == "G") {
                group = &cmd.back();
            } else if (cmd.front() == "usemtl") {
                material = &cmd.back();
            } else if (cmd.front() == "f" || cmd.front() == "F") {
                if (cmd.size() < 4) throw std::runtime_error("Command 'f' should have at least 3 arguments");
                Face face = Face((int)(cmd.size() - 1));
                face.setGroup(*group);
                face.setMaterial(*material);
                for (int i = 0; i < cmd.size(); i++) {
                    std::list<std::string> face_cmd = FileMaker::utils::split(cmd.back(), '/');
                }
            }
        }
        file.close();
    }
    else std::cout << "Unable to open file";
}

std::vector<Voxel> FileMaker::loadFile::loadSchematic(const std::string& fileName) {
    std::vector<Voxel> voxels;
    std::ifstream input("../schematics/" + fileName, std::ios::in | std::ios::binary);
    if (input.is_open()) {
        auto magic = FileMaker::utils::readUnsignedShort(input);
        if (magic != 0xFAAB) throw std::runtime_error("Invalid schematic magic");
        auto paletteSize = FileMaker::utils::readUnsignedShort(input);
        std::vector<int> palette;
        for (int i = 0; i < paletteSize; i++) {
            palette.push_back(FileMaker::utils::readRGB(input));
//            std::cout << std::hex << std::uppercase << palette.back() << std::endl;
        }
        while (true) {
            int color;
            int x = FileMaker::utils::readUnsignedShort(input);
            if (input.eof()) break;
            int y = FileMaker::utils::readUnsignedShort(input);
            int z = FileMaker::utils::readUnsignedShort(input);
            if (!palette.empty()) {
                color = palette[FileMaker::utils::readUnsignedShort(input)];
            } else {
                color = FileMaker::utils::readRGB(input);
            }
            voxels.emplace_back(x, y, z, color);
        }
    }
    return voxels;
}

void FileMaker::loadFile::saveSchematic(const std::string& fileName, std::vector<Voxel>& voxels) {
    std::ofstream output("../schematics/" + fileName, std::ios::out | std::ios::binary);
    if (output.is_open()) {
        FileMaker::utils::preprocessVoxels(voxels);
        std::vector<int> palette;
        FileMaker::utils::writeUnsignedShort(output, 0xFAAB);
        FileMaker::utils::writeUnsignedShort(output, (unsigned short)palette.size());
        for (int i = 0; i < palette.size(); i++) {
            palette.push_back(FileMaker::utils::writeRGB(output, palette[i]));
//            std::cout << std::hex << std::uppercase << palette.back() << std::endl;
        }
        for (const auto & voxel : voxels) {
            int color;
            FileMaker::utils::writeUnsignedShort(output, voxel.getX());
            FileMaker::utils::writeUnsignedShort(output, voxel.getY());
            FileMaker::utils::writeUnsignedShort(output, voxel.getZ());
            if (!palette.empty()) {
                FileMaker::utils::writeUnsignedShort(output, (unsigned short)(find(palette.begin(), palette.end(), voxel.getColor()) - palette.begin()));
            } else {
                FileMaker::utils::writeRGB(output, voxel.getColor());
            }
        }
    }
}

std::list<std::string> FileMaker::utils::split(std::string &str, char delimiter) {
    std::list<std::string> result;
    int start = 0;
    for (int j = 0; j < str.length(); j++) {
        if (str[j] == delimiter) {
            result.push_back( str.substr(start, j));
            start = j + 1;
        }
    }
    if (start < str.length()) {
        result.push_back(str.substr(start, str.length()));
    }
    return result;
}

unsigned short FileMaker::utils::readUnsignedShort(std::ifstream &input) {
    unsigned short v;
    input.read(reinterpret_cast<char *>(&v), sizeof(v));
    v = swap_endian(v);
    return v;
}
void FileMaker::utils::writeUnsignedShort(std::ofstream &output, unsigned short v) {
    v = swap_endian(v);
    output.write(reinterpret_cast<char *>(&v), sizeof(v));
}

int FileMaker::utils::readRGB(std::ifstream &input) {
    int v;
    input.read(reinterpret_cast<char *>(&v), 3);
    v = (v & 0xFF) << 16 | (v & 0xFF00) | (v & 0xFF0000) >> 16;
    return v;
}

int FileMaker::utils::writeRGB(std::ofstream &output, int v) {
    v = (v & 0xFF) << 16 | (v & 0xFF00) | (v & 0xFF0000) >> 16;
    output.write(reinterpret_cast<char *>(&v), 3);
}

void FileMaker::utils::preprocessVoxels(std::vector<Voxel>& voxels) {
    std::set<int> colors;
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int min_z = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    int max_z = INT_MIN;
    // Sets base coordinates and sets base points
    for (const Voxel& voxel : voxels) {
        if (voxel.getX() > max_x) max_x = voxel.getX();
        if (voxel.getY() > max_y) max_y = voxel.getY();
        if (voxel.getZ() > max_z) max_z = voxel.getZ();
        if (voxel.getX() < min_x) min_x = voxel.getX();
        if (voxel.getY() < min_y) min_y = voxel.getY();
        if (voxel.getZ() < min_z) min_z = voxel.getZ();
        colors.insert(voxel.getColor());
    }
    // Changes current coordinates to start at base point (0, 0, 0)
    for (Voxel voxel : voxels) {
        voxel.setX(voxel.getX() - min_x);
        voxel.setY(voxel.getY() - min_y);
        voxel.setZ(voxel.getZ() - min_z);
    }
    if (colors.size() < 0xFF) {
        auto paletteSize = colors.size();
        auto palette = std::vector<int>(paletteSize);
        for (int color : colors) {
            palette.push_back(color);
        }
    } else {
        auto paletteSize = 0;
    }
}