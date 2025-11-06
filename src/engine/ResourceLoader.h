#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "Model.h"
#include "Shader.h"
#include "ShaderReloader.h"
#include "Texture2D.h"
#include "stb_image.h"

// -- Todo : Unload Models

class ResourceLoader {
   public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures2D;
    static std::map<std::string, Model> Models;
    static std::map<int, ShaderReloader> Reloaders;

    static Shader LoadShader(const char* vFile, const char* fFile, const char* gFile, std::string name);
    static Shader& GetShader(std::string name);

    static Texture2D LoadTexture2D(const char* file, std::string name, bool sRGB = false);
    static Texture2D& GetTexture2D(std::string name);

    static Model LoadModel(const char* file, std::string name);
    static Model& GetModel(std::string name);

    // -- De-allocate everything properly.
    static void Clear();

   private:
    ResourceLoader();  // -- private constructor cause it's static and we
                       // don't want to be able to instance it

    static Shader LoadShaderFromFile(const char* vertexPath, const char* fragmentPath,
                                     const char* geometryShaderPath = nullptr);

    static Texture2D LoadTexture2DFromFile(const char* filePath, bool sRGB = false);
    static Model LoadModelFromFile(const char* filePath);
};
