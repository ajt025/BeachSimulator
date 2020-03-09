//
//  Terrain.h
//  BeachSimulator
//
//  Created by Anthony Joseph Tran on 3/8/20.
//  Copyright © 2020 Anthony Joseph Tran. All rights reserved.
//

#ifndef Terrain_h
#define Terrain_h

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

const float groundPlane[] = {
    -1000.0f, 0.0f, -1000.0f,
    -1000.0f, 0.0f, 1000.0f,
    1000.0f, 0.0f, 1000.0f,
    1000.0f, 0.0f, -1000.0f
};

class Terrain
{
public:
    GLuint vao, vbo, texID;
    
    void draw();
    
    Terrain();
    ~Terrain();
};

#endif /* Terrain_h */
