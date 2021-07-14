#include "vao.hh"

Vao::Vao()
        : vaoId(0), vboData{ 0, 0 }, vertices{}
{
    glGenVertexArrays(1, &vaoId); TEST_OPENGL_ERROR()
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()

    glGenBuffers(1, &vboData.verticeBufferId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData.verticeBufferId); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); TEST_OPENGL_ERROR()
    glGenBuffers(1, &vboData.normalBufferId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData.normalBufferId); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); TEST_OPENGL_ERROR()

    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
    glBindVertexArray(0); TEST_OPENGL_ERROR()
}

void Vao::addObjData(const ObjLoader::ObjData &objData) {
    unsigned verticesCount = objData.faces.size() * 3;
    unsigned normalsCount = objData.faces.size() * 3;
    vertices.reserve(verticesCount);
    normals.reserve(normalsCount);

    for (auto i = 0U; i < objData.faces.size(); ++i) {
        unsigned verticeIndex = (objData.faces[i].verticeIndex - 1) * 3;
        unsigned normalIndex = (objData.faces[i].normalIndex - 1) * 3;
        for (auto j = 0; j < 3; ++j) {
            vertices.push_back(objData.vertices[verticeIndex + j]);
            normals.push_back(objData.normals[normalIndex + j]);
        }
    }

    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData.verticeBufferId); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData.normalBufferId); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW); TEST_OPENGL_ERROR()

    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
    glBindVertexArray(0); TEST_OPENGL_ERROR()
}

void Vao::bindToProgram(const program &program, const char *verticeName, const char *normalName) {
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    if (verticeName) {
        glBindBuffer(GL_ARRAY_BUFFER, vboData.verticeBufferId); TEST_OPENGL_ERROR()
        GLint posLoc = glGetAttribLocSafe(program.get_program(), verticeName);
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr); TEST_OPENGL_ERROR()
        glEnableVertexAttribArray(posLoc);
    }
    if (normalName) {
        glBindBuffer(GL_ARRAY_BUFFER, vboData.normalBufferId); TEST_OPENGL_ERROR()
        GLint posLoc = glGetAttribLocSafe(program.get_program(), normalName);
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr); TEST_OPENGL_ERROR()
        glEnableVertexAttribArray(posLoc);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
    glBindVertexArray(0); TEST_OPENGL_ERROR()
}

void Vao::draw() {
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()); TEST_OPENGL_ERROR()
    glBindVertexArray(0); TEST_OPENGL_ERROR()
}