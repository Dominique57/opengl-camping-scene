#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool initGlew();
bool initGlfw(GLFWwindow* &window);
bool initGl();

bool initOpenglAndContext(GLFWwindow* &window);
