#pragma once

#include <vector>

#include "GameObject.h"
#include "Shader.h"
#include "Texture2D.h"
#include "glm.hpp"

struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float Lifetime;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Lifetime(1.0f) {}
};

class ParticleEmitter {
   public:
    ParticleEmitter(Shader shader, Texture2D texture, unsigned int amount);
    void Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f));
    void Render();

   private:
    std::vector<Particle> particles;
    unsigned int amount;
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    void Init();
    unsigned int FirstAvailableParticle();
    void RespawnParticle(Particle& particle, GameObject& object, glm::vec3 offset = glm::vec3(0.0f));
};