#include "Particles.h"

ParticleEmitter::ParticleEmitter(Shader shader, Texture2D texture, unsigned int amount) {
    this->shader = shader;
    this->texture = texture;
    this->amount = amount;
    this->Init();
}

void ParticleEmitter::Init() {
    LOG("Init Particle Emitter...");
    unsigned int VBO;
    float particle_quad[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                             0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    float particle_quad2[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                              0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad2), particle_quad2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    for (int i = 0; i < this->amount; ++i) {
        this->particles.push_back(Particle());
    }
}

void ParticleEmitter::Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset) {
    for (int i = 0; i < newParticles; ++i) {
        int particle = FirstAvailableParticle();
        RespawnParticle(particles[particle], object, offset);
    }

    for (unsigned int i = 0; i < this->amount; ++i) {
        Particle& p = particles[i];

        p.Lifetime -= deltaTime;
        if (p.Lifetime > 0.0) {
            p.Position -= p.Velocity * deltaTime;
            p.Color.a -= deltaTime * 2.5f;
        }
    }
}

void ParticleEmitter::Render() {
    // -- Additive Blend Mode

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shader.Use();

    for (auto p : particles) {
        if (p.Lifetime < 0)
            continue;

        // p.Color = glm::vec4(1.0, 0.3, 0.3, 1.0);
        shader.SetVec2("offset", p.Position);
        shader.SetVec4("color", p.Color);
        shader.SetFloat("scale", 5.0f);
        texture.Bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleEmitter::FirstAvailableParticle() {
    for (unsigned int i = lastUsedParticle; i < amount; ++i) {
        if (particles[i].Lifetime <= 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (particles[i].Lifetime <= 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    lastUsedParticle = 0;
    return 0;
}

void ParticleEmitter::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset) {
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = object.Position + glm::vec2(random) + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Lifetime = 0.5f;
    particle.Velocity = object.Velocity * 0.1f;
}
