// Created by dominique on 1/16/22.

#pragma once

#include <wrappers/glWrapper.hh>
#include "program.hh"

class Quad {
public:
    Quad();

    void draw();

protected:
    GLuint vaoId;
    GLuint vboId;
};

