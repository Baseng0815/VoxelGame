#pragma once

#include <GL/glew.h>

struct GeometryComponent {
    unsigned int drawCount;    

    //bool useElementDrawing;
    GLuint vao, vbo, ebo;

    bool buffersInitialized = false;
};
