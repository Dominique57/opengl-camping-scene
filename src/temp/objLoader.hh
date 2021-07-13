#pragma once

#include <string>
#include <vector>
#include "wrappers/glWrapper.hh"

class ObjLoader {
public:
    struct ObjData {
        std::vector<GLfloat> vertices;
        std::vector<unsigned> faces;
    };

public:
    ObjLoader() = default;

    static ObjData loadObj(const std::string &path);
};
