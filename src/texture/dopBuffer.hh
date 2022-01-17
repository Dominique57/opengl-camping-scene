// Created by dominique on 1/17/22.

#pragma once

#include <wrappers/glWrapper.hh>

class DopBuffer {

public:
    DopBuffer(int screen_w, int screen_h);

    GLuint getFboId() const;

    GLuint getColorTexId() const;

protected:
    GLuint fboId;
    GLuint colorTexId;
};

