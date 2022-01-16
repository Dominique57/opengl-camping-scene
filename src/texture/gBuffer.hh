// Created by dominique on 1/16/22.

#pragma once

#include <wrappers/glWrapper.hh>
#include <wrappers/glmWrapper.hh>

class GBuffer {

public:
    GBuffer(int screen_w, int screen_h);

protected:
    GLuint fboId;

    GLuint positionTexId;
    GLuint normalTexId;
    GLuint albedoTexId;

    GLuint rboId;
};

