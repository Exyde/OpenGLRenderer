#pragma once
#include "Shader.h"
#include "Texture2D.h"
enum class MeshPrimitive { Quad, Cube };

class MeshRenderer {
   public:
    MeshRenderer(Shader& shader);
    ~MeshRenderer();

    void DrawSprite(const Texture2D& texture, glm::vec2 pos, glm::vec2 size = glm::vec2(10.0f), float rotate = 0.0f,
                    glm::vec4 color = glm::vec4(1.0f));

    void DrawMesh(const Texture2D& texture, glm::vec2 pos, glm::vec2 size = glm::vec2(10.0f), float rotate = 0.0f,
                  glm::vec4 color = glm::vec4(1.0f));

    void UpdateShader(Shader& shader);

   private:
    Shader shader;
    void InitRenderData();
    unsigned int quadVAO;
    unsigned int cubeVao;
    unsigned int sphereVao;  // -- Todo
};