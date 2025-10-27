#include "BallObject.h"

BallObject::BallObject() : GameObject(), Radius(12.5f), Locked(true) {}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f), sprite, glm::vec4(1.0), velocity), Radius(radius), Locked(true) {}

glm::vec2 BallObject::Move(float dt, unsigned int windowWidth) {
    if (this->Locked)
        return this->Position;

    this->Position += this->Velocity * dt;  // -- Todo improve this + gravity + stuff fun

    // -- Check For Bounds
    if (CheckForLeftBound()) {
        this->Velocity.x = -this->Velocity.x;
        this->Position.x = 0.0;
    } else if (CheckForRightBound(windowWidth)) {
        this->Velocity.x = -this->Velocity.x;
        this->Position.x = windowWidth - this->Size.x;
    }

    if (CheckForTopBound()) {
        this->Velocity.y = -this->Velocity.y;
        this->Position.y = 0.0f;
    }

    return this->Position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
    this->Position = position;
    this->Velocity = velocity;
    this->Locked = true;
}
