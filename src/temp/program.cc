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

program* program::make_program_path(const std::vector<program::ShaderSource>& shaders_path) {
    auto shaders_src = shaders_path;
    for (auto& shader_src : shaders_src) {
        shader_src.src = readFile(shader_src.src);
    }
    return make_program(shaders_src);

}

program* program::make_program(const std::vector<program::ShaderSource>& shaders) {
    auto res = new program(glCreateProgram()); TEST_OPENGL_ERROR();
    auto shaders_id = std::vector<GLuint>{};

    try {
        // Compile & link
        for (const auto& shader : shaders)
            shaders_id.push_back(compileShader(shader.src, shader.type, shader.name));
        linkShader(res->program_, shaders_id);
    } catch (const std::exception& e) {
        // Create error state
        res->program_ = 0;
        res->log_ = e.what();
        res->ready_ = false;

        // Delete ressources
        for (auto shader : shaders_id)
            glDeleteShader(shader); TEST_OPENGL_ERROR();
        glDeleteProgram(res->program_); TEST_OPENGL_ERROR();
        return res;
    }

    // Free compile-needed shaders ressources
    for (auto shader_id : shaders_id)
        glDeleteShader(shader_id); TEST_OPENGL_ERROR();

    res->ready_ = true;
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
