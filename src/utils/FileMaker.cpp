//
// Created by bogum on 14.04.2022.
//

#include <iostream>
#include "FileMaker.h"
#include "Face.h"
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <array>


void FileMaker::testFM::test() {
    FileMaker::loadObject("test.obj");

}

void FileMaker::loadFile::loadObject(std::string fileName) {
    std::list<double>vertices;
    std::list<double> textures;
    std::list<double> normals;
    std::string material = nullptr;
    std::string group = nullptr;
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
                group = cmd.back();
            } else if (cmd.front() == "usemtl") {
                material = cmd.back();
            } else if (cmd.front() == "f" || cmd.front() == "F") {
                if (cmd.size() < 4) throw std::runtime_error("Command 'f' should have at least 3 arguments");
                Face face = Face((int)(cmd.size() - 1));
                face.setGroup(group);
                face.setMaterial(material);
                for (int i = 0; i < cmd.size(); i++) {
                    std::list<std::string> face_cmd = FileMaker::utils::split(cmd.back(), '/');
                }
            }
        }
        file.close();
    }
    else std::cout << "Unable to open file";
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


