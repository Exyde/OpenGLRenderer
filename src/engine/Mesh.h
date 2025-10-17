#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "Texture.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Tex {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
   public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Tex> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Tex> textures);
    void Draw(Shader& shader);

   private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif