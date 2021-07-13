#include "vao.hh"

Vao::Vao()
        : vaoId(0), vboData{ 0 }, vertices{}
{
    glGenVertexArrays(1, &vaoId); TEST_OPENGL_ERROR()
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()

    glGenBuffers(1, &vboData.vertexBufferId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData.vertexBufferId); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
    glBindVertexArray(0); TEST_OPENGL_ERROR()
}

void Vao::addObjData(const ObjLoader::ObjData &objData) {
    unsigned verticesCount = objData.faces.size() * 3;
    vertices.reserve(verticesCount);

    for (auto i = 0U; i < objData.faces.size(); ++i) {
        for (auto j = 0; j < 3; ++j) {
            unsigned indexStart = (objData.faces[i] - 1) * 3;
            vertices.push_back(objData.vertices[indexStart + j]);
        }
    }

    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData.vertexBufferId); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW); TEST_OPENGL_ERROR()

    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
    glBindVertexArray(0); TEST_OPENGL_ERROR()
}

void Vao::bindToProgram(const program &program, const char *vertexName) {
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData.vertexBufferId); TEST_OPENGL_ERROR()
    if (vertexName) {
        GLint posLoc = glGetAttribLocSafe(program.get_program(), vertexName);
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