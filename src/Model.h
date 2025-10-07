#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class Model{
public:
    Model (std::string const &path) {loadModel(path);}
    void Draw (Shader &shader) {for (unsigned int i =0; i< meshes.size(); i++) { meshes[i].Draw(shader);}};

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Tex> textures_loaded; 

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Tex> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


#endif