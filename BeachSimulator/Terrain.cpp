//
//  Terrain.cpp
//  BeachSimulator
//
//  Created by Anthony Joseph Tran on 3/8/20.
//  Copyright © 2020 Anthony Joseph Tran. All rights reserved.
//

#include "Terrain.h"

#include "stb_image.h"

Terrain::Terrain() {
    
    // Initialize buffer arrays
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundPlane), &groundPlane, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // LOAD JPGs
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("dry_sand.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "dry_sand.jpg texture failed to load!" << std::endl;
        stbi_image_free(data);
    }
}

void Terrain::draw() {
    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Bind to saved texture
    glBindTexture(GL_TEXTURE_2D, texID);
    
    // Draw points
    glDrawArrays(GL_QUADS, 0, 4);
    
    // Unbind from the VAO.
    glBindVertexArray(0);
}
