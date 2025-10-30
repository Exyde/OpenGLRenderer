#pragma once

#include "GameObject.h"

const glm::vec2 POWERUP_SIZE(60.0, 20.0);
const glm::vec2 VELOCITY(0.0, 150.0F);

enum class PowerUpType { Speed, Sticky, PassThrough, PadSizeIncrease, Confuse, Chaos };

class PowerUp : public GameObject {
   public:
    PowerUpType Type;
    float Duration;
    bool Active;

    PowerUp(PowerUpType type, glm::vec4 color, float duration, glm::vec2 position, Texture2D& texture)
        : GameObject(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Active() {}
};
