#include "Game.h"

Game::Game(unsigned int width, unsigned int height)
    : State(ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {}

void Game::Initialize() {}

void Game::ProcessInput(float deltaTime) {}

void Game::Update(float deltaTime) {}

void Game::Render() {
    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
