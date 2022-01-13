#pragma once

#include <cassert>
#include <functional>
#include <string>
#include <vector>
#include "temp/init_gl.hh"

#define TEST_OPENGL_ERROR()   { \
    GLenum err = glGetError();  \
    if (err != GL_NO_ERROR) {   \
        std::cerr << "OpenGL ERROR (code: " << err << ") at " \
            << __FILE__ << ":" <<  __LINE__ << std::endl;     \
        assert(false && "OPENGL FAILED, crashing process !"); \
    }                           \
}

#define IGNORE_OPENGL_ERROR()   {   \
    GLenum err = glGetError();      \
    for (; err != GL_NO_ERROR; err = glGetError()) \
        continue;                   \
    }

std::vector<GLchar> getShaderLog(GLint shader);
std::vector<GLchar> getProgramLog(GLint program);

GLuint compileShader(const std::string& shader, GLenum shader_type,
        std::string err=std::string());
void linkShader(GLuint program, const std::vector<GLuint>& shaders);

GLint glGetUniformLocSafe(GLuint program, const char* name);
GLint glGetAttribLocSafe(GLuint program, const char* name);