#pragma once

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// clang-format on

enum GameState { ACTIVE, MENU, PAUSED, WIN };

class Game {
   public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Initialize();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
};