#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>


class Texture {
public:
    GLuint ID;
    int width, height, channels;

    Texture (const char* filePath, int clampMode, bool useAlpha);
    ~Texture();
    void Bind();


};


#endif