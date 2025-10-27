#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader) {
    this->shader = shader;
    InitRenderData();
}

SpriteRenderer::~SpriteRenderer() { glDeleteVertexArrays(1, &this->quadVAO); }

void SpriteRenderer::DrawSprite(const Texture2D& texture, glm::vec2 pos,
                                glm::vec2 size, float angle, glm::vec4 color) {
    this->shader.Use();

    // -- Translation
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(pos, 0.0f));

    // -- Rotation

    model =
        glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
    model =
        glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // -- Scaling
    model = glm::scale(model, glm::vec3(size, 1.0));

    this->shader.SetMat4("model", model);
    this->shader.SetVec4("tint", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData() {
    // -- Quad vertices (pos, tc)
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &this->quadVAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)0);

    // -- Unbind --
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
