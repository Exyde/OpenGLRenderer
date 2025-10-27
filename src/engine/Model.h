#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "Logger.h"
#include "Mesh.h"
#include "Shader.h"

class Model {
   public:
    Model(std::string path) { loadModel(path); }
    void Draw(Shader& shader);

   private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Tex> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Tex> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                          std::string typeName);
    unsigned int TextureFromFile(const char* path,
                                 const std::string& directory);
};