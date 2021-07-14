#pragma once

#include <string>
#include <vector>
#include "wrappers/glWrapper.hh"

class ObjLoader {
public:
    struct VertexDataIndex {
        int verticeIndex = -1;
        int textureIndex = -1;
        int normalIndex = -1;
    };

    struct ObjData {
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        std::vector<VertexDataIndex> faces;
    };

public:
    ObjLoader() = default;

    static ObjData loadObj(const std::string &path);
};

std::istream& operator>>(std::istream& in, ObjLoader::VertexDataIndex& vertexDataIndex);