#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Shader& shader) {
    this->shader = shader;
    InitRenderData();
}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::DrawMesh(const Texture2D& texture, glm::vec2 pos, glm::vec2 size, float rotate, glm::vec4 color) {}

void MeshRenderer::UpdateShader(Shader& shader) {
    this->shader = shader;
}

void MeshRenderer::InitRenderData() {
    // -- Init all Vao Types
}
