#include "ResourceLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "stb_image.h"

std::map<std::string, Texture2D> ResourceLoader::Textures2D;
std::map<std::string, Shader> ResourceLoader::Shaders;

Shader ResourceLoader::LoadShader(const char* vShaderFile,
                                  const char* fShaderFile,
                                  const char* gShaderFile, std::string name) {
    Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceLoader::GetShader(std::string name) { return Shaders[name]; }

Texture2D ResourceLoader::LoadTexture2D(const char* file, bool alpha,
                                        std::string name) {
    Textures2D[name] = LoadTexture2DFromFile(file, alpha);
    return Textures2D[name];
}

Texture2D ResourceLoader::GetTexture2D(std::string name) {
    return Textures2D[name];
}

void ResourceLoader::Clear() {
    for (auto iter : Shaders) glDeleteProgram(iter.second.ID);
    for (auto iter : Textures2D) glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceLoader::LoadShaderFromFile(const char* vertexPath,
                                          const char* fragmentPath,
                                          const char* geometryShaderPath) {
    Shader shader(vertexPath, fragmentPath);
    return shader;
}

Texture2D ResourceLoader::LoadTexture2DFromFile(const char* filePath,
                                                bool useAlpha) {
    Texture2D texture;

    if (useAlpha) {
        texture.InternalFormat = GL_RGBA;
        texture.ImageFormat = GL_RGBA;
    }

    int width, height, channels;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);

    if (data) {
        std::cout << "[TEXTURE] Loaded : " << filePath << std::endl;
        texture.Generate(width, height, data);
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    return texture;
}
