#include "GameObject.h"

GameObject::GameObject()
    : Position(0.0), Size(1.0), Velocity(0), Color(1.0f), Rotation(0), Sprite(), IsSolid(false), IsDestroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec4 color, glm::vec2 velocity)
    : Position(pos),
      Size(size),
      Velocity(velocity),
      Color(color),
      Sprite(sprite),
      Rotation(0.0f),
      IsSolid(false),
      IsDestroyed(false) {}

GameObject::~GameObject() {}

void GameObject::Draw(SpriteRenderer& renderer) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

// -- Bounds --
bool GameObject::CheckForLeftBound() {
    return this->Position.x <= 0.0f;
}

bool GameObject::CheckForRightBound(unsigned int windowWidth) {
    return (this->Position.x + this->Size.x >= windowWidth);
}

bool GameObject::CheckForTopBound() {
    return (this->Position.y <= 0.0);
}

bool GameObject::CheckForBottomBound(unsigned int windowHeight) {
    return (this->Position.y + this->Size.y >= windowHeight);
}
