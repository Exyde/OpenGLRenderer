#pragma once

#include <glad/glad.h>

class Texture2D {
   public:
    GLuint ID;
    unsigned int Width, Height;
    unsigned int InternalFormat;  // -- TextureObject Format
    unsigned int ImageFormat;     // -- Loaded Image Format

    unsigned int Wrap_S, Wrap_T, Filter_Min, Filter_Mag;

    Texture2D();
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Bind() const;
};