#pragma once

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// clang-format on
#include <vector>

#include "../engine/Logger.h"
#include "../engine/ResourceLoader.h"
#include "../engine/SpriteRenderer.h"
#include "../glm/glm.hpp"
#include "BallObject.h"
#include "GameLevel.h"

enum GameState { ACTIVE, MENU, PAUSED, WIN };

class Game {
   public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int currentLevel;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Initialize();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
};