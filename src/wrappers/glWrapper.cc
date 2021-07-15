#include "glWrapper.hh"
#include <stdexcept>
#include <vector>

std::vector<GLchar> getShaderLog(GLint shader) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    auto infoLog = std::vector<GLchar>(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

    return infoLog;
}

std::vector<GLchar> getProgramLog(GLint program) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    auto infoLog = std::vector<GLchar>(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    return infoLog;
}

GLuint compileShader(const std::string& shader_src, GLenum shader_type,
        std::string err) {
    GLuint shader = glCreateShader(shader_type);
    auto shader_str = shader_src.c_str();
    glShaderSource(shader, 1, &shader_str, 0);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        auto infoLog = getShaderLog(shader);

        glDeleteShader(shader);

        throw std::runtime_error(err + ":\n" + infoLog.data());
    }

    return shader;
}

void linkShader(GLuint program, std::initializer_list<GLuint> shaders) {
    for (auto shader : shaders)
        glAttachShader(program, shader);
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        auto infoLog = getProgramLog(program);

        glDeleteProgram(program);
        for (auto shader : shaders)
            glDeleteShader(shader);

        throw std::runtime_error(infoLog.data());
    }

    for (auto shader : shaders)
        glDetachShader(program, shader);
}

GLint glGetUniformLocSafe(GLuint program, const char* name) {
    GLint res = glGetUniformLocation(program, name); TEST_OPENGL_ERROR()
    std::cerr << program << " " << name << " " << res << std::endl;
    if (res < 0) {
        auto msg = std::string("Requested uniform `") + name
            + "` not found !";
        std::cerr << msg << std::endl;
        return res;
        throw std::invalid_argument(msg.c_str());
    }

    return res;
}

GLint glGetAttribLocSafe(GLuint program, const char* name) {
    GLint res = glGetAttribLocation(program, name); TEST_OPENGL_ERROR()
    if (res < 0) {
        auto msg = std::string("Requested attrib `") + name
            + "` not found !";
        std::cerr << msg << std::endl;
        return res;
        throw std::invalid_argument(msg.c_str());
    }

    return res;
}
