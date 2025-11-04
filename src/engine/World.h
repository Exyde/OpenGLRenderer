#pragma once
#include <map>

#include "WorldObject.h"
class World {
   public:
    static int GUID;
    static std::map<int, WorldObject> WorldObjects;

    static void Init();
    static void Update(float dt);
    static void Render();
    static void Clear();

   private:
    World();
};