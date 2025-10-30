#pragma once

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// clang-format on
#include <vector>

#include "../engine/Direction.h"
#include "../engine/Logger.h"
#include "../engine/Particles.h"
#include "../engine/PostProcessor.h"
#include "../engine/ResourceLoader.h"
#include "../engine/SpriteRenderer.h"
#include "../glm/glm.hpp"
#include "BallObject.h"
#include "GameLevel.h"
#include "PowerUp.h"
#include "algorithm"

struct Collision {
    bool collided;
    Direction direction;
    glm::vec2 penetration;
};

enum class GameState { ACTIVE, MENU, PAUSED, WIN };

class Game {
   public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    std::vector<PowerUp> PowerUps;
    unsigned int currentLevel;

    Game(unsigned int width, unsigned int height);
    ~Game();

    // -- Core Loop
    void Initialize();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void Exit();

    // -- Collision
    bool CheckCollisionAABB(GameObject& A, GameObject& B);
    Collision CheckCollisionSphereBox(BallObject& A, GameObject& B);
    void ComputeCollisions();

    // -- Level
    void ResetLevel();
    void ResetPlayer();

    // -- Powers Ups

    void SpawnPowerUps(GameObject& block);
    void UpdatePowersUps(float deltaTime);
    Texture2D& GetPowerUpTexture(PowerUpType type);

   private:
    std::map<PowerUpType, Texture2D> powerUpsTextures;
};