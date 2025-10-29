#pragma once

// #define WIND_WAKER

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// clang-format on

#include <cmath>
#include <iostream>
#include <thread>

#include "engine/Engine.h"
#include "game/Game.h"

constexpr glm::vec4 ColorWhite = glm::vec4(1.0);
constexpr glm::vec4 ColorBlack = glm::vec4(0.0);
constexpr glm::vec4 ColorGray = glm::vec4(0.5);

enum class ApplicationMode { Engine, Game };
ApplicationMode appMode = ApplicationMode::Game;

const uint32_t SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
uint32_t CURRENT_WIDTH = 800, CURRENT_HEIGHT = 600;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
Engine engine(SCREEN_WIDTH, SCREEN_HEIGHT);

bool FIRST_MOUSE = true;
float lastMouseX = 400;
float lastMouseY = 300;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool cursorVisible = false;

void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height);
void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
void Mouse_Callback(GLFWwindow* window, double xposin, double yposin);
bool InitializeGLFW();
GLFWwindow* CreateWindow(uint32_t width, uint32_t height);
void InitIMGUI(GLFWwindow* window);
