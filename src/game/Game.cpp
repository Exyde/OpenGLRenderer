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
    // -- Log
    LOG_INFO(LogCategory::Game, "Initializing...");

    // -- Create Orthographic Matrix
    float w = static_cast<float>(this->Width);
    float h = static_cast<float>(this->Height);
    glm::mat4 projection = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

    // -- Load & Setup Main Shader
    ResourceLoader::LoadShader("Shaders/sprite.vs", "Shaders/sprite.fs",
                               nullptr, "spriteShader");
    ResourceLoader::GetShader("spriteShader").Use();
    ResourceLoader::GetShader("spriteShader").SetInt("sprite", 0);
    ResourceLoader::GetShader("spriteShader").SetMat4("projection", projection);

    // -- Setup Sprite Renderer
    Renderer = new SpriteRenderer(ResourceLoader::GetShader("spriteShader"));

    // -- Load Textures
    ResourceLoader::LoadTexture2D("Resources/Textures/awesomeface.png", true,
                                  "face");
    ResourceLoader::LoadTexture2D("Resources/Textures/block.png", false,
                                  "block");
    ResourceLoader::LoadTexture2D("Resources/Textures/block_solid.png", false,
                                  "block_solid");

    // -- Load Levels
    GameLevel levelOne;
    levelOne.Load("Resources/Levels/level1.txt", this->Width, this->Height / 2);

    this->Levels.push_back(levelOne);
    this->currentLevel = 0;

    // -- Log
    LOG_INFO(LogCategory::Game, "Initialized !");
}

void Game::ProcessInput(float deltaTime) {}

void Game::Update(float deltaTime) {}

void Game::Render() {
    glClearColor(0.1, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    this->Levels[this->currentLevel].Draw(*Renderer);
}
