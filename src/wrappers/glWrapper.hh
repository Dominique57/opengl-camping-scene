#pragma once

#include <functional>
#include <string>
#include <vector>
#include "temp/init_gl.hh"

std::vector<GLchar> getShaderLog(GLint shader);
std::vector<GLchar> getProgramLog(GLint program);

GLuint compileShader(const std::string& shader, GLenum shader_type,
        std::string err=std::string());
void linkShader(GLuint program, std::initializer_list<GLuint> shaders);

GLint glGetUniformLocSafe(GLuint program, const char* name);
GLint glGetAttribLocSafe(GLuint program, const char* name);
