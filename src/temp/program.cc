#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "wrappers/glWrapper.hh"
#include "init_gl.hh"
#include "program.hh"

program::~program() {
    if (ready_)
        glDeleteProgram(program_);
}

void program::use() const {
    if (!isready())
        throw std::logic_error("Program is not ready, see logs!");

    glUseProgram(program_);
}

static inline std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::invalid_argument("File");

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

program* program::make_program_path(
        const std::string& vertex_path, const std::string& frag_path) {
    auto vertex_src = readFile(vertex_path);
    auto frag_src = readFile(frag_path);
    return make_program(vertex_src, frag_src);

}

program* program::make_program(
        const std::string& vertex, const std::string& fragment) {
    // https://www.khronos.org/opengl/wiki/Example/GLSL_Full_Compile_Linking
    // Create and compile vertexShader and fragmentShader
    auto res = new program(0, "", true);

    GLuint vertexShader;
    GLuint fragShader;
    try {
        vertexShader = compileShader(vertex, GL_VERTEX_SHADER, "VERTEX");
    } catch (const std::runtime_error& e) {
        res->log_ = std::string(e.what());
        res->ready_ = false;
        return res;
    }

    try {
        fragShader = compileShader(fragment, GL_FRAGMENT_SHADER, "FRAGMENT");
    } catch (const std::runtime_error& e) {
        glDeleteShader(vertexShader);
        res->log_ = std::string(e.what());
        res->ready_ = false;
        return res;
    }

    GLuint program = glCreateProgram();
    try {
        linkShader(program, {vertexShader, fragShader});
    } catch (const std::runtime_error& e) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
        res->log_ = std::string(e.what());
        res->ready_ = false;
        return res;
    }

    res->program_ = program;
    return res;
}

void program::setUniformMat4(const char *name,
        const glm::mat4& val, bool throwIfMissing) const {
    GLint eltLoc = glGetUniformLocSafe(program_, name);
    if (eltLoc != -1) {
        glProgramUniformMatrix4fv(program_, eltLoc, 1, GL_FALSE, &val[0][0]); TEST_OPENGL_ERROR()
    } else if (throwIfMissing) {
        throw std::invalid_argument("Name does not exist in shader !");
    }
}

void program::setUniformVec3(const char *name, const glm::vec3 &val,
                             bool throwIfMissing) const {
    GLint eltLoc = glGetUniformLocSafe(program_, name);
    if (eltLoc != -1) {
        glProgramUniform3fv(program_, eltLoc, 1, &val[0]); TEST_OPENGL_ERROR()
    } else if (throwIfMissing) {
        throw std::invalid_argument("Name does not exist in shader !");
    }
}

void program::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}

void program::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void program::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}
