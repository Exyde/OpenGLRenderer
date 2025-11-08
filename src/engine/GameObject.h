#pragma once

#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "Texture2D.h"
#include "glm/glm.hpp"

class GameObject {
   public:
    glm::vec3 Position, Size, Velocity;
    glm::vec4 Color;
    float Rotation;
    bool IsSolid;
    bool IsDestroyed;
    Texture2D Sprite;
    MeshPrimitive MeshType;

    // -- Methods
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec4 color = glm::vec4(1.0f),
               glm::vec3 velocity = glm::vec3(0.0f), MeshPrimitive meshType = MeshPrimitive::Quad);
    ~GameObject();

    virtual void Draw(SpriteRenderer& renderer);
    virtual void Draw(MeshRenderer& renderer);

    bool CheckForLeftBound();
    bool CheckForRightBound(unsigned int windowWidth);
    bool CheckForTopBound();
    bool CheckForBottomBound(unsigned int windowHeight);
};