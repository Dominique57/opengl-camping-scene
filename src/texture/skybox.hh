#pragma once

#include "temp/program.hh"
#include "temp/stb_image.h"

class Skybox
{
public:
    Skybox(std::vector<std::string> faces);
    ~Skybox() = default;
    void draw();
    void bindToProgram(const program &program);
private:
    static constexpr float skyboxVertices[] = {
            // positions
            -200.0f,  200.0f, -200.0f,
            -200.0f, -200.0f, -200.0f,
            200.0f, -200.0f, -200.0f,
            200.0f, -200.0f, -200.0f,
            200.0f,  200.0f, -200.0f,
            -200.0f,  200.0f, -200.0f,

            -200.0f, -200.0f,  200.0f,
            -200.0f, -200.0f, -200.0f,
            -200.0f,  200.0f, -200.0f,
            -200.0f,  200.0f, -200.0f,
            -200.0f,  200.0f,  200.0f,
            -200.0f, -200.0f,  200.0f,

            200.0f, -200.0f, -200.0f,
            200.0f, -200.0f,  200.0f,
            200.0f,  200.0f,  200.0f,
            200.0f,  200.0f,  200.0f,
            200.0f,  200.0f, -200.0f,
            200.0f, -200.0f, -200.0f,

            -200.0f, -200.0f,  200.0f,
            -200.0f,  200.0f,  200.0f,
            200.0f,  200.0f,  200.0f,
            200.0f,  200.0f,  200.0f,
            200.0f, -200.0f,  200.0f,
            -200.0f, -200.0f,  200.0f,

            -200.0f,  200.0f, -200.0f,
            200.0f,  200.0f, -200.0f,
            200.0f,  200.0f,  200.0f,
            200.0f,  200.0f,  200.0f,
            -200.0f,  200.0f,  200.0f,
            -200.0f,  200.0f, -200.0f,

            -200.0f, -200.0f, -200.0f,
            -200.0f, -200.0f,  200.0f,
            200.0f, -200.0f, -200.0f,
            200.0f, -200.0f, -200.0f,
            -200.0f, -200.0f,  200.0f,
            200.0f, -200.0f,  200.0f
    };
    unsigned int skyboxVAO;
    unsigned int skyboxVBO;
    unsigned int cubemapTexture;
};


unsigned int loadCubemap(std::vector<std::string> faces);