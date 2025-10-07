#include "Texture.h"

Texture::Texture(const char* filePath, int clampMode, bool useAlpha, std::string type){
    glGenTextures(1, &ID);
    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load(filePath, &width, &height, &channels, 0);

    this->type = type;

    if (data){
        if (useAlpha){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, data);

        } else{
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

    } else{
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


}

void Texture::Bind(){ glBindTexture (GL_TEXTURE_2D, ID);}

Texture::~Texture(){}