#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <iostream>

#include "stb_image.h"

class Texture {
   public:
    GLuint ID;
    int width, height, channels;
    std::string type;

    Texture(const char* filePath, int clampMode, bool useAlpha,
            std::string type);
    ~Texture();
    void Bind();
};

#endif