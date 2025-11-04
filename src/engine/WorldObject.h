#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture2D.h"
#include "glm.hpp"

// -- Should support VAO & Model Datas ?

struct Transform {
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;
};

class WorldObject {
   public:
    WorldObject() = default;
    // virtual ~WorldObject() = default;
    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();

   private:
    Shader shader;
    Transform t;
};