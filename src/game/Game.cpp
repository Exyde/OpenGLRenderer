#include "Game.h"

#include "../engine/Logger.h"
#include "../engine/ResourceLoader.h"
#include "../engine/SpriteRenderer.h"
#include "../glm/glm.hpp"

SpriteRenderer* Renderer;

Game::Game(unsigned int width, unsigned int height)
    : State(ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {}

void Game::Initialize() {
    LOG_INFO(LogCategory::Game, "Initializing...");

    ResourceLoader::LoadShader("Shaders/sprite.vs", "Shaders/sprite.fs",
                               nullptr, "spriteShader");

    float w = static_cast<float>(this->Width);
    float h = static_cast<float>(this->Height);
    glm::mat4 projection = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

    glm::mat4 perspective = glm::perspective(45.0f, w / h, 0.1f, 100.0F);

    ResourceLoader::GetShader("spriteShader").Use();
    ResourceLoader::GetShader("spriteShader").SetInt("sprite", 0);
    ResourceLoader::GetShader("spriteShader").SetMat4("projection", projection);

    Renderer = new SpriteRenderer(ResourceLoader::GetShader("spriteShader"));
    ResourceLoader::LoadTexture2D("Resources/Textures/awesomeface.png", true,
                                  "face");

    LOG_INFO(LogCategory::Game, "Initialized !");
}

void Game::ProcessInput(float deltaTime) {}

void Game::Update(float deltaTime) {}

void Game::Render() {
    glClearColor(0.1, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    Renderer->DrawSprite(ResourceLoader::GetTexture2D("face"),
                         glm::vec2(200.0, 200.0), glm::vec2(100.0, 100.0),
                         45.0f, glm::vec4(1.0, 0.0, 0.0f, 1.0f));
}
