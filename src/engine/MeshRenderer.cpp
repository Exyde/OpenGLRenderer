#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Shader& shader) {
    this->shader = shader;
    InitRenderData();
}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::DrawMesh(const Texture2D& texture, glm::vec3 pos, glm::vec3 size, float rotate, glm::vec4 color) {
    this->shader.Use();

    // -- Translation
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, pos);

    // -- Rotation

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0, 0.0, 1.0));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // -- Scaling
    model = glm::scale(model, size);

    // -- Adapt to phong i Guess
    this->shader.SetMat4("model", model);
    this->shader.SetVec4("tint", color);

    glActiveTexture(GL_TEXTURE0);
    // texture.Bind();
    ResourceLoader::GetTexture2D("skybox").Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void MeshRenderer::UpdateShader(Shader& shader) {
    this->shader = shader;
}

void MeshRenderer::InitRenderData() {
    InitCubeData();
    InitQuadData();
    InitSphereData();
}

void MeshRenderer::InitCubeData() {}

void MeshRenderer::InitQuadData() {
    // -- Quad Datas

    const float quadVertices[] = {
        // positions           // normals           // texCoords
        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right

        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f   // top-right
    };

    glGenVertexArrays(1, &this->quadVAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void MeshRenderer::InitSphereData() {}
