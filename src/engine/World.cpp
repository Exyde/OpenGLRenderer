#include "World.h"

std::map<int, WorldObject> World::WorldObjects;

void World::Init() {}

void World::Update(float dt) {
    for (auto [_, o] : WorldObjects) o.Update(dt);
}

void World::Render() {
    for (auto [_, o] : WorldObjects) o.Render();
}

void World::Clear() {}
