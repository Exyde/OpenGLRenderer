#pragma once

#include <map>
#include <string>

#include "Shader.h"
#include "Texture2D.h"

class ResourceLoader {
   public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures2D;

    static Shader LoadShader(const char* vFile, const char* fFile,
                             const char* gFile, std::string name);
    static Shader& GetShader(std::string name);

    static Texture2D LoadTexture2D(const char* file, bool useAlpha,
                                   std::string name);
    static Texture2D& GetTexture2D(std::string name);

    // -- De-allocate everything properly.
    static void Clear();

   private:
    ResourceLoader();  // -- private constructor cause it's static and we
                       // don't want to be able to instance it

    static Shader LoadShaderFromFile(const char* vertexPath,
                                     const char* fragmentPath,
                                     const char* geometryShaderPath = nullptr);

    static Texture2D LoadTexture2DFromFile(const char* filePath, bool useAlpha);
};
