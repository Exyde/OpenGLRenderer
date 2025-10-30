#include "BallObject.h"

BallObject::BallObject() : GameObject(), Radius(12.5f), Locked(true), PassThrough(false), Sticky(false) {}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f), sprite, glm::vec4(1.0), velocity),
      Radius(radius),
      Locked(true),
      PassThrough(false),
      Sticky(false) {}

glm::vec2 BallObject::Move(float dt, unsigned int windowWidth) {
    if (this->Locked)
        return this->Position;

    this->Position += this->Velocity * dt;  // -- Todo improve this + gravity + stuff fun

    // -- Check For Bounds
    if (CheckForLeftBound()) {
        this->Velocity.x = -this->Velocity.x * 1.2f;
        this->Position.x = 0.0;
    } else if (CheckForRightBound(windowWidth)) {
        this->Velocity.x = -this->Velocity.x * 1.2f;
        this->Position.x = windowWidth - this->Size.x;
    }

    if (CheckForTopBound()) {
        this->Velocity.y = -this->Velocity.y * 2.0;
        this->Position.y = 0.0f;
    }

    this->Velocity.y = glm::clamp(this->Velocity.y, -1500.0f, 1500.f);
    this->Velocity.x = glm::clamp(this->Velocity.x, -1500.0f, 1500.f);
    return this->Position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
    this->Position = position;
    this->Velocity = velocity;
    this->Locked = true;
    this->Sticky = false;
    this->PassThrough = false;
}
