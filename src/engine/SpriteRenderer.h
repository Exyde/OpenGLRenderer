#pragma once

#include "Shader.h"
#include "Texture2D.h"

// -- Could herit from Renderer ? To have Mesh Renderer per say
class SpriteRenderer {
   public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();

    void DrawSprite(const Texture2D& texture, glm::vec2 pos,
                    glm::vec2 size = glm::vec2(10.0f), float rotate = 0.0f,
                    glm::vec4 color = glm::vec4(1.0f));

   private:
    Shader shader;
    unsigned int quadVAO;
    void InitRenderData();
};