#pragma once

#include <vector>
#include "temp/objLoader.hh"
#include "temp/program.hh"
#include "wrappers/glWrapper.hh"

class Vao {
private:
    struct VboData {
        GLuint vertexBufferId;
    };
public:
    Vao();

    void addObjData(const ObjLoader::ObjData &objData);

    void bindToProgram(const program &program, const char *vertexName);

    void draw();

private:
    GLuint vaoId;
    VboData vboData;
    std::vector<GLfloat> vertices;
};