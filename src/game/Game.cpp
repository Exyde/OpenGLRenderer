#include "Game.h"

// -- Player
const glm::vec2 PLAYER_SIZE(100.0F, 20.0f);
const float PLAYER_VELOCITY(500.0f);

// -- Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

GameObject* Player;
SpriteRenderer* Renderer;
BallObject* Ball;
ParticleEmitter* trailFX;
const std::string PLAYER_TEXTURE = "player";

Game::Game(unsigned int width, unsigned int height) : State(GameState::ACTIVE), Keys(), Width(width), Height(height) {
    LOG_INFO(LogCategory::Game, "Creating Game Mode...");
}

Game::~Game() {
    delete Renderer;
    delete Player;
    delete Ball;
    delete trailFX;
}

void Game::Exit() {
    ResourceLoader::Clear();
    glfwTerminate();
}

void Game::Initialize() {
    // -- Log
    LOG_INFO(LogCategory::Game, "Initializing Game Mode...");

    // -- Init Open GL States
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // -- Create Orthographic Matrix
    float w = static_cast<float>(this->Width);
    float h = static_cast<float>(this->Height);
    glm::mat4 projection = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

    // -- Load & Setup Main Shader
    ResourceLoader::LoadShader("Shaders/sprite.vs", "Shaders/sprite.fs", nullptr, "spriteShader");
    ResourceLoader::GetShader("spriteShader").Use();
    ResourceLoader::GetShader("spriteShader").SetMat4("projection", projection);
    ResourceLoader::GetShader("spriteShader").SetInt("sprite", 0);

    ResourceLoader::LoadShader("Shaders/particle.vs", "Shaders/particle.fs", nullptr, "particle");
    ResourceLoader::GetShader("particle").Use();
    ResourceLoader::GetShader("particle").SetMat4("projection", projection);
    ResourceLoader::GetShader("particle").SetInt("sprite", 0);

    // -- Setup Sprite Renderer
    Renderer = new SpriteRenderer(ResourceLoader::GetShader("spriteShader"));

    // -- Load Textures
    ResourceLoader::LoadTexture2D("Resources/Textures/paddle.png", true, PLAYER_TEXTURE);
    ResourceLoader::LoadTexture2D("Resources/Textures/awesomeface.png", true, "face");
    ResourceLoader::LoadTexture2D("Resources/Textures/block.png", false, "block");
    ResourceLoader::LoadTexture2D("Resources/Textures/block_solid.png", false, "block_solid");
    ResourceLoader::LoadTexture2D("Resources/Textures/background.jpg", false, "background");
    ResourceLoader::LoadTexture2D("Resources/Textures/skybox/back.png", false, "skybox");
    ResourceLoader::LoadTexture2D("Resources/Textures/particle.png", true, "particle");

    // -- Create Player
    glm::vec2 playerPos(this->Width / 2.0f - PLAYER_SIZE.x / 2.0F, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceLoader::GetTexture2D(PLAYER_TEXTURE));

    // -- Ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceLoader::GetTexture2D("skybox"));

    // -- Particles
    trailFX =
        new ParticleEmitter(ResourceLoader::GetShader("particle"), ResourceLoader::GetTexture2D("particle"), 1000);

    // -- Load Levels
    GameLevel levelOne, levelTwo, levelThree, levelFour;
    levelOne.Load("Resources/Levels/level1.txt", this->Width, this->Height / 2);
    levelTwo.Load("Resources/Levels/level2.txt", this->Width, this->Height / 2);
    levelThree.Load("Resources/Levels/level3.txt", this->Width, this->Height / 2);
    levelFour.Load("Resources/Levels/level4.txt", this->Width, this->Height / 2);

    this->Levels.push_back(levelOne);
    this->Levels.push_back(levelTwo);
    this->Levels.push_back(levelThree);
    this->Levels.push_back(levelFour);
    this->currentLevel = 1;

    // -- Log
    LOG_INFO(LogCategory::Game, "Initialized !");
}

void Game::ProcessInput(float deltaTime) {
    if (this->State == GameState::ACTIVE) {
        float vel = PLAYER_VELOCITY * deltaTime;

        // -- Change Level
        if (this->Keys[GLFW_KEY_O]) {
            currentLevel++;
            if (currentLevel > (Levels.size() - 1))
                currentLevel = 0;
        }

        // -- Unlock Ball
        if (this->Keys[GLFW_KEY_SPACE]) {
            Ball->Locked = false;
        }

        // -- Left Motion
        if (this->Keys[GLFW_KEY_A]) {
            if (Player->Position.x >= 0) {
                Player->Position.x -= vel;

                if (Ball->Locked)
                    Ball->Position.x -= vel;
            }
        }

        // -- Right Motion
        if (this->Keys[GLFW_KEY_D]) {
            if (Player->Position.x <= this->Width - Player->Size.x) {
                Player->Position.x += vel;

                if (Ball->Locked)
                    Ball->Position.x += vel;
            }
        }
    }
}

void Game::Update(float deltaTime) {
    Ball->Move(deltaTime, this->Width);
    this->ComputeCollisions();

    if (Ball->CheckForBottomBound(this->Height)) {
        this->ResetLevel();
        this->ResetPlayer();
    }

    trailFX->Update(deltaTime, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
}

void Game::Render() {
    glClearColor(0.1, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (this->State == GameState::ACTIVE) {
        Renderer->DrawSprite(ResourceLoader::GetTexture2D("skybox"), glm::vec2(0.0f, 0.0f),
                             glm::vec2(this->Width, this->Height), 0.0f);
        this->Levels[this->currentLevel].Draw(*Renderer);
        Player->Draw(*Renderer);
        trailFX->Render();
        Ball->Draw(*Renderer);
    }
}

bool Game::CheckCollisionAABB(GameObject& A, GameObject& B) {
    bool collisionX = (A.Position.x + A.Size.x) >= B.Position.x && B.Position.x + B.Size.x >= A.Position.x;
    bool collisionY = (A.Position.y + A.Size.y) >= B.Position.y && B.Position.y + B.Size.y >= A.Position.y;
    return collisionX && collisionY;
}

Collision Game::CheckCollisionSphereBox(BallObject& A, GameObject& B) {
    // -- Circle
    auto center = A.Position + A.Radius;

    // -- AABB Center & Half-Extents
    glm::vec2 aabb_halfextents(B.Size.x / 2.0f,
                               B.Size.y / 2.0f);  // -- You could cache this but also that's why it's
                                                  // expsenive to recompute bounds everytime you change
                                                  // transform
    glm::vec2 aabb_center(B.Position.x + aabb_halfextents.x, B.Position.y + aabb_halfextents.y);

    // -- Difference (D Vector)
    glm::vec2 D = center - aabb_center;
    glm::vec2 clamped = glm::clamp(D, -aabb_halfextents, aabb_halfextents);

    glm::vec2 closestPoint = aabb_center + clamped;
    glm::vec2 circleCenterToBoxEdge = closestPoint - center;

    // -- Collision if Inside

    Collision coll;
    if (glm::length(circleCenterToBoxEdge) < A.Radius) {
        coll.collided = true;
        coll.direction = GetVectorClosestDirection(circleCenterToBoxEdge);
        coll.penetration = circleCenterToBoxEdge;
    } else {
        coll.collided = false;
        coll.direction = Direction::UP;
        coll.penetration = glm::vec2(0.0);
    }

    return coll;
}

void Game::ComputeCollisions() {
    for (GameObject& other : this->Levels[this->currentLevel].Bricks) {
        if (!other.IsDestroyed) {
            Collision collision = CheckCollisionSphereBox(*Ball, other);

            if (collision.collided) {
                if (!other.IsSolid)
                    other.IsDestroyed = true;

                if (collision.direction == Direction::LEFT || collision.direction == Direction::RIGHT) {
                    Ball->Velocity.x = -Ball->Velocity.x;
                    float penetration = Ball->Radius - std::abs(collision.penetration.x);
                    if (collision.direction == Direction::LEFT) {
                        Ball->Position.x += penetration;
                    } else {
                        Ball->Position.x -= penetration;
                    }
                } else {
                    Ball->Velocity.y = -Ball->Velocity.y;
                    float penetration = Ball->Radius - std::abs(collision.penetration.y);

                    if (collision.direction == Direction::UP)
                        Ball->Position.y -= penetration;
                    else
                        Ball->Position.y += penetration;
                }
            }
        }
    }

    // -- Player
    Collision result = CheckCollisionSphereBox(*Ball, *Player);

    if (!Ball->Locked && result.collided) {
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float dst = (Ball->Position.x + Ball->Radius) - centerBoard;
        float pct = dst / (Player->Size.x / 2.0f);
        float strength = 2.0f;

        glm::vec2 oldVel = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * pct * strength;
        Ball->Velocity.y = -1.0 * abs(Ball->Velocity.y);  // -- sticky paddle hack
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVel);
    }
}

void Game::ResetLevel() {
    if (this->currentLevel == 0)
        this->Levels[0].Load("Resources/Levels/level1.txt", this->Width, this->Height / 2);
    else if (this->currentLevel == 1)
        this->Levels[1].Load("Resources/Levels/level2.txt", this->Width, this->Height / 2);
    else if (this->currentLevel == 2)
        this->Levels[2].Load("Resources/Levels/level3.txt", this->Width, this->Height / 2);
    else if (this->currentLevel == 3)
        this->Levels[3].Load("Resources/Levels/level4.txt", this->Width, this->Height / 2);
}

void Game::ResetPlayer() {
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)),
                INITIAL_BALL_VELOCITY);
}