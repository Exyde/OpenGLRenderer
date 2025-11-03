#include "ResourceLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "stb_image.h"

std::map<std::string, Texture2D> ResourceLoader::Textures2D;
std::map<std::string, Shader> ResourceLoader::Shaders;
std::map<std::string, Model> ResourceLoader::Models;
std::map<Shader, ShaderReloader> ResourceLoader::Reloaders;

Shader ResourceLoader::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile,
                                  std::string name) {
    Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader& ResourceLoader::GetShader(std::string name) {
    return Shaders[name];
}

Texture2D ResourceLoader::LoadTexture2D(const char* file, std::string name) {
    Textures2D[name] = LoadTexture2DFromFile(file);
    return Textures2D[name];
}

Texture2D& ResourceLoader::GetTexture2D(std::string name) {
    return Textures2D[name];
}

Model ResourceLoader::LoadModel(const char* file, std::string name) {
    Models[name] = LoadModelFromFile(file);
    return Models[name];
}

Model& ResourceLoader::GetModel(std::string name) {
    return Models[name];
}

void ResourceLoader::Clear() {
    for (auto iter : Shaders) glDeleteProgram(iter.second.ID);
    for (auto iter : Textures2D) glDeleteTextures(1, &iter.second.ID);
    // for (auto iter : Models)
}

Shader ResourceLoader::LoadShaderFromFile(const char* vertexPath, const char* fragmentPath,
                                          const char* geometryShaderPath) {
    Shader shader(vertexPath, fragmentPath);

    ShaderReloader reloader(shader);
    Reloaders[shader] = reloader;
    return shader;
}

Texture2D ResourceLoader::LoadTexture2DFromFile(const char* filePath) {
    Texture2D texture;

    int width, height, channels;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);

    if (channels == 4) {
        texture.InternalFormat = GL_RGBA;
        texture.ImageFormat = GL_RGBA;
    } else if (channels == 1) {
        texture.InternalFormat = GL_RED;
        texture.ImageFormat = GL_RED;
    } else {
        texture.InternalFormat = GL_RGB;
        texture.ImageFormat = GL_RGB;
    }

    if (data) {
        LOG_INFO(LogCategory::Texture, " Loaded : ", filePath);
        texture.Generate(width, height, data);
        stbi_image_free(data);
    } else {
        LOG_ERROR(LogCategory::Texture, "Failed to load texture : ", filePath);
    }
    return texture;
}

Model ResourceLoader::LoadModelFromFile(const char* filePath) {
    auto model = Model(filePath);
    return model;
}
