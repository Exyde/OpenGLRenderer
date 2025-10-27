#include "Game.h"

#include "../engine/Logger.h"
#include "../engine/ResourceLoader.h"
#include "../engine/SpriteRenderer.h"
#include "../glm/glm.hpp"

const glm::vec2 PLAYER_SIZE(100.0F, 20.0f);
const float PLAYER_VELOCITY(500.0f);

GameObject* Player;
SpriteRenderer* Renderer;

const std::string PLAYER_TEXTURE = "player";

Game::Game(unsigned int width, unsigned int height) : State(ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {}

void Game::Initialize() {
    // -- Log
    LOG_INFO(LogCategory::Game, "Initializing...");

    // -- Create Orthographic Matrix
    float w = static_cast<float>(this->Width);
    float h = static_cast<float>(this->Height);
    glm::mat4 projection = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

    // -- Load & Setup Main Shader
    ResourceLoader::LoadShader("Shaders/sprite.vs", "Shaders/sprite.fs", nullptr, "spriteShader");
    ResourceLoader::GetShader("spriteShader").Use();
    ResourceLoader::GetShader("spriteShader").SetInt("sprite", 0);
    ResourceLoader::GetShader("spriteShader").SetMat4("projection", projection);

    // -- Setup Sprite Renderer
    Renderer = new SpriteRenderer(ResourceLoader::GetShader("spriteShader"));

    // -- Load Textures
    ResourceLoader::LoadTexture2D("Resources/Textures/paddle.png", true, PLAYER_TEXTURE);
    ResourceLoader::LoadTexture2D("Resources/Textures/awesomeface.png", true, "face");
    ResourceLoader::LoadTexture2D("Resources/Textures/block.png", false, "block");
    ResourceLoader::LoadTexture2D("Resources/Textures/block_solid.png", false, "block_solid");
    ResourceLoader::LoadTexture2D("Resources/Textures/background.jpg", false, "background");
    ResourceLoader::LoadTexture2D("Resources/Textures/skybox/back.png", false, "skybox");

    // -- Create Player
    glm::vec2 playerPos(this->Width / 2.0f - PLAYER_SIZE.x / 2.0F, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceLoader::GetTexture2D(PLAYER_TEXTURE));

    // -- Load Levels
    GameLevel levelOne, levelTwo, levelThree, levelFour;
    levelOne.Load("Resources/Levels/level1.txt", this->Width, this->Height / 2);
    levelTwo.Load("Resources/Levels/level2.txt", this->Width, this->Height / 2);
    levelThree.Load("Resources/Levels/level3.txt", this->Width, this->Height / 2);
    levelFour.Load("Resources/Levels/level4.txt", this->Width, this->Height / 2);

    this->Levels.push_back(levelOne);
    this->Levels.push_back(levelTwo);
    this->Levels.push_back(levelThree);
    this->Levels.push_back(levelFour);
    this->currentLevel = 1;

    // -- Log
    LOG_INFO(LogCategory::Game, "Initialized !");
}

void Game::ProcessInput(float deltaTime) {
    if (this->State == ACTIVE) {
        float vel = PLAYER_VELOCITY * deltaTime;

        if (this->Keys[GLFW_KEY_A]) {
            if (Player->Position.x >= 0)
                Player->Position.x -= vel;
        }

        if (this->Keys[GLFW_KEY_O]) {
            currentLevel++;
            if (currentLevel > (Levels.size() - 1))
                currentLevel = 0;
        }

        if (this->Keys[GLFW_KEY_D]) {
            if (Player->Position.x <= this->Width - Player->Size.x) {
                Player->Position.x += vel;
            }
        }
    }
}

void Game::Update(float deltaTime) {}

void Game::Render() {
    glClearColor(0.1, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (this->State == ACTIVE) {
        Renderer->DrawSprite(ResourceLoader::GetTexture2D("skybox"), glm::vec2(0.0f, 0.0f),
                             glm::vec2(this->Width, this->Height), 0.0f);
        this->Levels[this->currentLevel].Draw(*Renderer);
        Player->Draw(*Renderer);
    }
}
