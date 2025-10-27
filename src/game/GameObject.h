#pragma once

#include "../engine/SpriteRenderer.h"
#include "../engine/Texture2D.h"
#include "glm/glm.hpp"

class GameObject {
   public:
    glm::vec2 Position, Size, Velocity;
    glm::vec4 Color;
    float Rotation;
    bool IsSolid;
    bool IsDestroyed;
    // -- Maybe one day hold it's renderer ? or shader ? humm
    Texture2D Sprite;

    // -- Methods
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f),
               glm::vec2 velocity = glm::vec2(0.0f));
    ~GameObject();

    virtual void Draw(SpriteRenderer& renderer);

    bool CheckForLeftBound();
    bool CheckForRightBound(unsigned int windowWidth);
    bool CheckForTopBound();
    bool CheckForBottomBound(unsigned int windowHeight);
};