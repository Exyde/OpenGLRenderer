#include "Engine.h"

GameObject* TestObj;
MeshRenderer* Renderer;

Engine::~Engine() {}

Engine::Engine(unsigned int width, unsigned int height)
    : State(EngineState::ACTIVE), Keys(), Width(width), Height(height) {
    LOG_INFO(LogCategory::Engine, "Creating Engine Mode...");
}

#pragma region LoadingResources

std::vector<std::string> GetImagesPath(const std::string& rootDir) {
    std::vector<std::string> paths;

    for (const auto& entry : fs::recursive_directory_iterator(rootDir)) {
        if (entry.is_regular_file()) {
            auto ext = entry.path().extension().string();
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                paths.push_back(entry.path().string());
            }
        }
    }
    return paths;
}

void Engine::LoadTextures() {
    // -- Load Jam Textures from main root
    auto imagesPaths = GetImagesPath("Resources/Textures/CoreImages/Art");
    int imageID = 0;

    for (const auto& path : imagesPaths) {
        std::string filename = std::filesystem::path(path).stem().string();
        // ResourceLoader::LoadTexture2D(path.c_str(), filename, true);
    }
    LOG("FOUND IMAGES : ", imagesPaths.size());

    // -- Load Textures
    ResourceLoader::LoadTexture2D("Resources/Textures/skybox/back.png", "skybox");
    ResourceLoader::LoadTexture2D("Resources/Textures/grass.png", "grass", true);
    ResourceLoader::LoadTexture2D("Resources/Textures/container_diffuse.png", "diffuse", true);
    ResourceLoader::LoadTexture2D("Resources/Textures/container_specular.png", "specular");
    ResourceLoader::LoadTexture2D("Resources/Textures/container_emmisive.jpg", "emissive");

    // -- Skybox
    std::vector<std::string> cubemapPaths{
        "Resources/Textures/skybox/right.png", "Resources/Textures/skybox/left.png",
        "Resources/Textures/skybox/top.png",   "Resources/Textures/skybox/back.png",
        "Resources/Textures/skybox/front.png", "Resources/Textures/skybox/back.png",
    };

    std::vector<std::string> cubemapPathsBox{
        "Resources/Textures/wow.jpg", "Resources/Textures/wow.jpg", "Resources/Textures/wow.jpg",
        "Resources/Textures/wow.jpg", "Resources/Textures/wow.jpg", "Resources/Textures/wow.jpg",
    };
    cubemapTexture = LoadCubeMap(cubemapPathsBox);
}

void Engine::LoadShaders() {
    // -- Load & Setup Main Shader
    ResourceLoader::LoadShader("Shaders/sprite.vs", "Shaders/sprite.fs", nullptr, "spriteShader");
    ResourceLoader::GetShader("spriteShader").Use();
    ResourceLoader::GetShader("spriteShader").SetInt("sprite", 0);

    // -- Shaders
    ResourceLoader::LoadShader("Shaders/vert.vs", "Shaders/frag.fs", nullptr, "phong");
    ResourceLoader::LoadShader("Shaders/grass.vs", "Shaders/grass.fs", nullptr, "grass");
    ResourceLoader::LoadShader("Shaders/light.vs", "Shaders/light.fs", nullptr, "light");
    ResourceLoader::LoadShader("Shaders/postprocess.vs", "Shaders/postprocess.fs", nullptr, "postProcess");

    ResourceLoader::LoadShader("Shaders/skybox.vs", "Shaders/skybox.fs", nullptr, "skybox");
    ResourceLoader::GetShader("skybox").Use().SetInt("skybox", 0);
}

void LoadMeshes() {
    ResourceLoader::LoadModel("Resources/Models/backpack/backpack.obj", "backpack");
}

#pragma endregion
void Engine::InitStatics() {
    pointLightAmbient = glm::vec3(0.05f);
    pointLightDiffuse = glm::vec3(0.0f, 0.2f, 0.7f);
    pointLightSpecular = glm::vec3(1.0f);
    flashLightAmbient = glm::vec3(0.05f);
    flashLightDiffuse = glm::vec3(0.8f, 0.2f, 0.6f);
    flashLightSpecular = glm::vec3(1.0f);
    LightPosition = glm::vec3(-2.0f, 1.5F, 3.3F);
    cam = Camera(Vector3(0.0f, 0.0f, 0.0f));
    checkInterval = std::chrono::milliseconds(500);

    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
}

void Engine::InitOpenGlSettings() {
    // -- Depth Buffer Config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Vertex Shader Config
    glEnable(GL_PROGRAM_POINT_SIZE);

    // -- Initial Culling Config
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CCW);

    // -- Stencil Buffer
    // glEnable(GL_STENCIL_TEST);
    // glStencilMask(0xFF);  // -- Write as 1
    // glStencilMask(0x00);  // -- Write as 0
}

void Engine::Initialize() {
    // -- Log
    LOG_INFO(LogCategory::Engine, "Initializing Engine Mode...");
    stbi_set_flip_vertically_on_load(true);
    float w = static_cast<float>(this->Width);
    float h = static_cast<float>(this->Height);

    InitStatics();
    LoadTextures();
    LoadShaders();
    LoadMeshes();
    InitOpenGlSettings();

    Renderer = new MeshRenderer(ResourceLoader::GetShader("phong"));
    sprRenderer = new SpriteRenderer(ResourceLoader::GetShader("spriteShader"));

    for (int i = 0; i < 50; i++) {
        glm::vec3 pos(i * 10, i * 5, 1);
        glm::vec3 size(10.0);

        CoreImagesPlanes.push_back(
            new GameObject(pos, size, Texture2D(), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec3(0.1)));
    }

#pragma region CUBE AND LIGHTS VAOS VBOS

    // -- Skybox --
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // -- NDC Quad
    glGenVertexArrays(1, &ndcQuadVAO);
    glGenBuffers(1, &ndcQuadVBO);
    glBindVertexArray(ndcQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ndcQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(NDCQuadVertices), &NDCQuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // -- Quad Datas
    glGenVertexArrays(1, &QuadVAO);
    glGenBuffers(1, &QuadVBO);
    glBindVertexArray(QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // -- Cube Datas ---
    glGenVertexArrays(1, &CubeVAO);
    glGenBuffers(1, &CubeVBO);
    glBindVertexArray(CubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // -- Light Data
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

#pragma endregion
#pragma region FrameBuffers

    // -- FrameBuffers

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
    // -- You could also attach a depth-stencil buffer to this
    // framebuffer with a 32 bit texture (24 bit for depth, 8 for
    // Stencil)
    // -- Use a RenderBufferObject for Depth & Stencil as Read it's read
    // only
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // -- To be valid, attach at least on buffer, one color attachment,
    // all attachements should be complete, each buffer same samples
    // count.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_ERROR(LogCategory::Engine, "FrameBuffer is not complete !");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion

    LOG_INFO(LogCategory::Game, "Initialized !");
}

void Engine::ProcessInput(float deltaTime) {
    if (this->Keys[GLFW_KEY_UP]) {
        userUpDown += 0.001;
    }
    if (this->Keys[GLFW_KEY_DOWN]) {
        userUpDown -= 0.001;
    }

    if (this->Keys[GLFW_KEY_LEFT]) {
        userLeftRight -= 0.001;
    }

    if (this->Keys[GLFW_KEY_RIGHT]) {
        userLeftRight += 0.001;
    }

    if (this->Keys[GLFW_KEY_W]) {
        cam.ProcessKeyboardInputs(FORWARD, deltaTime);
    }
    if (this->Keys[GLFW_KEY_S]) {
        cam.ProcessKeyboardInputs(BACKWARD, deltaTime);
    }
    if (this->Keys[GLFW_KEY_A]) {
        cam.ProcessKeyboardInputs(LEFT, deltaTime);
    }
    if (this->Keys[GLFW_KEY_D]) {
        cam.ProcessKeyboardInputs(RIGHT, deltaTime);
    }
    if (this->Keys[GLFW_KEY_Q]) {
        cam.ProcessKeyboardInputs(UP, deltaTime);
    }
    if (this->Keys[GLFW_KEY_E]) {
        cam.ProcessKeyboardInputs(DOWN, deltaTime);
    }
}

void Engine::Update(float deltaTime) {
    // -- FPS Computation --
    static double lastTime = glfwGetTime();
    static int frameCount = 0;
    double currentTime = glfwGetTime();
    frameCount++;

    if (currentTime - lastTime >= 1.0) {
        double fps = double(frameCount) / (currentTime - lastTime);
        frameCount = 0;
        lastTime = currentTime;

        currentFPS = fps;
    }

    auto now = std::chrono::steady_clock::now();
    if (now - lastCheck > checkInterval) {
        lastCheck = now;

        for (auto& [s, r] : ResourceLoader::Reloaders) {
            r.CheckForChanges();
        }
    }

#pragma region SunUpdate
    // -- Transformed positions
    float sunSpeed = deltaTime * userSunSpeed;
    if (updateSun)
        sunTheta += sunSpeed;
    glm::vec3 sunYaw = glm::vec3(cos(sunTheta) * sunRadius, 0, sin(sunTheta) * sunRadius);
    glm::vec3 sunPitch = glm::vec3(0, cos(sunTheta) * sunRadius, sin(sunTheta) * sunRadius);
    offsetedLightPos = LightPosition + sunYaw + sunPitch;
#pragma endregion

    World::Update(deltaTime);
}

void Engine::Render(float deltaTime) {
    if (this->State == EngineState::ACTIVE) {
        DrawCallsCounter = 0;

        // -- Our Goal
        World::Render();

#ifdef DEV_BUILD
        BeginImGuiFrame();
#endif
        InitScenePass();

        // -- Textures Units -- Binds for Basic boxes, but need to be upgraded.
        glActiveTexture(GL_TEXTURE0);
        ResourceLoader::GetTexture2D("diffuse").Bind();
        glActiveTexture(GL_TEXTURE1);
        ResourceLoader::GetTexture2D("specular").Bind();
        glActiveTexture(GL_TEXTURE2);
        ResourceLoader::GetTexture2D("emmisive").Bind();

        // -- Draw Scene
        SetupPhongShader(viewMatrix, offsetedLightPos);

        bool drawScene = true;
        if (drawScene) {
            DrawGround();
            //   DrawFloatingsCubes();
            DrawBackpack();
            // DrawGrass(viewMatrix);
            // DrawLightsObjects(viewMatrix, offsetedLightPos);
            DrawSkybox();
        }

        // -- Render all Core Images
        for (auto o : CoreImagesPlanes) o->Draw(*Renderer);

        // -- Post Process
        PostProcessPass();
#ifdef DEV_BUILD
        RenderImGUI();
#endif
    }
}

void Engine::Exit() {
    LOG("Exit engine...");
    glDeleteFramebuffers(1, &framebuffer);
    ResourceLoader::Clear();
}

void Engine::SetShaderLightsDatas(Shader& shader, glm::vec3 lightPos) {
    // -- Directionnal Light
    shader.SetVec3("dirLight.direction", lightPos);
    shader.SetVec3("dirLight.ambient", glm::vec3(ambientLightColor[0], ambientLightColor[1], ambientLightColor[2]));
    shader.SetVec3("dirLight.diffuse", glm::vec3(diffuseLightColor[0], diffuseLightColor[1], diffuseLightColor[2]));
    shader.SetVec3("dirLight.specular", glm::vec3(specularLightColor[0], specularLightColor[1], specularLightColor[2]));

    // -- Point Lights
    shader.SetVec3("pointLights[0].position", pointLightPositions[0]);
    shader.SetVec3("pointLights[0].ambient", pointLightAmbient);
    shader.SetVec3("pointLights[0].diffuse", pointLightDiffuse);
    shader.SetVec3("pointLights[0].specular", pointLightSpecular);
    shader.SetFloat("pointLights[0].constant", attenuation[0]);
    shader.SetFloat("pointLights[0].linear", attenuation[1]);
    shader.SetFloat("pointLights[0].quadratic", attenuation[2]);

    shader.SetVec3("pointLights[1].position", pointLightPositions[1]);
    shader.SetVec3("pointLights[1].ambient", pointLightAmbient);
    shader.SetVec3("pointLights[1].diffuse", pointLightDiffuse);
    shader.SetVec3("pointLights[1].specular", pointLightSpecular);
    shader.SetFloat("pointLights[1].constant", attenuation[0]);
    shader.SetFloat("pointLights[1].linear", attenuation[1]);
    shader.SetFloat("pointLights[1].quadratic", attenuation[2]);

    shader.SetVec3("pointLights[2].position", pointLightPositions[2]);
    shader.SetVec3("pointLights[2].ambient", pointLightAmbient);
    shader.SetVec3("pointLights[2].diffuse", pointLightDiffuse);
    shader.SetVec3("pointLights[2].specular", pointLightSpecular);
    shader.SetFloat("pointLights[2].constant", attenuation[0]);
    shader.SetFloat("pointLights[2].linear", attenuation[1]);
    shader.SetFloat("pointLights[2].quadratic", attenuation[2]);

    shader.SetVec3("pointLights[3].position", pointLightPositions[3]);
    shader.SetVec3("pointLights[3].ambient", pointLightAmbient);
    shader.SetVec3("pointLights[3].diffuse", pointLightDiffuse);
    shader.SetVec3("pointLights[3].specular", pointLightSpecular);
    shader.SetFloat("pointLights[3].constant", attenuation[0]);
    shader.SetFloat("pointLights[3].linear", attenuation[1]);
    shader.SetFloat("pointLights[3].quadratic", attenuation[2]);

    // Lamp Torch
    shader.SetVec3("flashLight.position", cam.Position.GLM());
    shader.SetVec3("flashLight.direction", cam.Front.GLM());
    shader.SetFloat("flashLight.cutOff", glm::cos(glm::radians(flashLightRadius)));
    shader.SetFloat("flashLight.outerCutOff", glm::cos(glm::radians(flashLightRadius + 2.5f)));
    shader.SetVec3("flashLight.ambient", flashLightAmbient);
    shader.SetVec3("flashLight.diffuse", flashLightDiffuse);
    shader.SetVec3("flashLight.specular", flashLightSpecular);
    shader.SetFloat("flashLight.constant", attenuation[0]);
    shader.SetFloat("flashLight.linear", attenuation[1]);
    shader.SetFloat("flashLight.quadratic", attenuation[2]);
}

void Engine::UpdatePostProcessFrameBuffer(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

unsigned int LoadCubeMap(std::vector<std::string> facePaths) {
    // -- Create texture
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int w, h, channels;
    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < facePaths.size(); i++) {
        unsigned char* data = stbi_load(facePaths[i].c_str(), &w, &h, &channels, 0);

        if (data) {
            LOG_INFO(LogCategory::Texture, " CUBEMAP Loaded : ", facePaths[i]);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            LOG_ERROR(LogCategory::Texture, "Cubemap failed to load at path", facePaths[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(true);
    return textureID;
}

void Engine::InitScenePass() {
    //-- FIRST PASS -- NORMAL SCENE BUT OF SCREEN RENDERING
    if (enablePostProcessing) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (projectionModeSelection == 0) {
        projectionMatrix = glm::perspective(glm::radians(cam.Fov), (float)Width / (float)Height, NearPlane, FarPlane);
    } else {
        projectionMatrix = glm::ortho(0.0f, (float)Width, 0.0f, (float)Height, -1.0f, 1.0f);
    }

    viewMatrix = cam.GetViewMatrix();
}

void Engine::DrawFloatingsCubes() {
    // Todo : Bind correct textures
    // -- Multiples Floating Cubes
    auto phongShader = ResourceLoader::GetShader("phong");
    glBindVertexArray(CubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        auto modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
        float angle = 20.0f * i;
        if (UI_rotateStuff) {
            modelMatrix =
                glm::rotate(modelMatrix, glm::radians(angle + ElapsedTime() * 100), glm::vec3(1.0f, 0.3f, 0.5f));
        }
        phongShader.SetMat4("model", modelMatrix);

        OpenGlDraw(GL_TRIANGLES, 0, 36);
    }
}

void Engine::DrawBackpack() {
    auto modelMatrix = glm::mat4(1.0F);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    auto phongShader = ResourceLoader::GetShader("phong");
    phongShader.SetMat4("model", modelMatrix);
    ResourceLoader::GetModel("backpack").Draw(phongShader);
}

void Engine::DrawGround() {
    //-- DrawGround
    auto modelMatrix = glm::mat4(1.0F);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(100, 1, 100));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1, 0));
    auto phongShader = ResourceLoader::GetShader("phong");
    phongShader.SetMat4("model", modelMatrix);
    glBindVertexArray(CubeVAO);
    OpenGlDraw(GL_TRIANGLES, 0, 36);
}

void Engine::DrawGrass(glm::mat4 viewMatrix) {
    // -- Grass
    auto grassShader = ResourceLoader::GetShader("grass");
    grassShader.Use();
    glActiveTexture(GL_TEXTURE0);
    ResourceLoader::GetTexture2D("grass").Bind();
    grassShader.SetMat4("view", viewMatrix);
    grassShader.SetMat4("projection", projectionMatrix);
    grassShader.SetInt("mat.diffuse", 0);
    // -- User Data
    grassShader.SetFloat("uTime", ElapsedTime() * userUpDown);
    grassShader.SetFloat("T", userLeftRight);
    grassShader.SetVec3("ViewPos", cam.Position.GLM());
    glBindVertexArray(QuadVAO);

    for (unsigned int i = 0; i < vegetation.size(); i++) {
        float grassCount = 8;
        for (int rot = 0; rot < grassCount; rot++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, vegetation[i]);
            float angle = (double)glm::two_pi<float>() / grassCount * rot;
            model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
            grassShader.SetMat4("model", model);
            OpenGlDraw(GL_TRIANGLES, 0, 6);
        }
    }
}

void Engine::DrawLightsObjects(glm::mat4 viewMatrix, glm::vec3 offsetedLightPos) {
    // -- Light Object -- //
    auto lightShader = ResourceLoader::GetShader("light").Use();
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, offsetedLightPos);
    lightModel = glm::scale(lightModel, glm::vec3(25.0f));
    lightShader.SetMat4("model", lightModel);
    lightShader.SetMat4("view", viewMatrix);
    lightShader.SetMat4("projection", projectionMatrix);
    lightShader.SetVec3("LightPos", offsetedLightPos);
    lightShader.SetVec3("LightColor", Vector3(diffuseLightColor[0], diffuseLightColor[1], diffuseLightColor[2]).GLM());
    glBindVertexArray(lightVAO);
    OpenGlDraw(GL_TRIANGLES, 0, 36);

    for (int i = 0; i < 4; i++) {
        lightModel = glm::mat4(1.0F);
        lightModel = glm::translate(lightModel, pointLightPositions[i]);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.SetMat4("model", lightModel);
        lightShader.SetVec3("LightPos", pointLightPositions[i]);
        lightShader.SetVec3("LightColor",
                            Vector3(pointLightDiffuse[0], pointLightDiffuse[1], pointLightDiffuse[2]).GLM());

        OpenGlDraw(GL_TRIANGLES, 0, 36);
    }
}

void Engine::DrawSkybox() {
    // -- Skybox
    glDepthFunc(GL_LEQUAL);
    auto skyboxShader = ResourceLoader::GetShader("skybox").Use();
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glm::mat4 skyboxViewWithoutTranslation = glm::mat4(glm::mat3(cam.GetViewMatrix()));
    skyboxShader.SetMat4("view", skyboxViewWithoutTranslation);
    skyboxShader.SetMat4("projection", projectionMatrix);
    OpenGlDraw(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

void Engine::SetupPhongShader(glm::mat4 viewMatrix, glm::vec3 offsetedLightPos) {
    // -- Phong Shader
    auto phongShader = ResourceLoader::GetShader("phong");
    phongShader.Use();
    phongShader.SetMat4("view", viewMatrix);
    phongShader.SetMat4("projection", projectionMatrix);
    if (lightModeSelection == 1)
        phongShader.SetBool("ToonShading", true);
    else
        phongShader.SetBool("ToonShading", false);

    // -- Material
    phongShader.SetInt("mat.diffuse", 0);
    phongShader.SetInt("mat.specular", 1);
    phongShader.SetInt("mat.emissive", 2);
    phongShader.SetFloat("mat.shininess", 32.0F);

    // -- User Data
    phongShader.SetFloat("uTime", ElapsedTime() * userUpDown);
    phongShader.SetFloat("T", userLeftRight);
    phongShader.SetVec3("ViewPos", cam.Position.GLM());

    SetShaderLightsDatas(phongShader, offsetedLightPos);
}

void Engine::PostProcessPass() {
    if (enablePostProcessing) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0F, 1.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        auto postProcessShader = ResourceLoader::GetShader("postProcess");
        postProcessShader.Use();
        postProcessShader.SetFloat("uTime", ElapsedTime() * userUpDown);
        postProcessShader.SetBool("uEnableChroma", postFX.enableChromaticAberration);
        postProcessShader.SetFloat("uChromaIntensity", postFX.chromaIntensity);
        postProcessShader.SetBool("uEnableInvert", postFX.enableInvert);
        postProcessShader.SetBool("uEnableGrayscale", postFX.enableGrayscale);
        postProcessShader.SetBool("uEnableKernel", postFX.enableKernel);
        postProcessShader.SetInt("uKernelType", postFX.kernelType);
        postProcessShader.SetInt("uCorrectGamma", postFX.correctGamma);

        glBindVertexArray(ndcQuadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, renderTexture);
        OpenGlDraw(GL_TRIANGLES, 0, 6);
    }
}

void Engine::BeginImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}
void Engine::RenderImGUI() {
#pragma region IMGUI
    ImGui::Begin("Scene");
    ImGui::End();

    ImGui::Begin("Buttons");
    if (ImGui::Button("Load Model (.obj)")) {
    }
    ImGui::End();

    ImGui::Begin("Inspector");

    if (ImGui::CollapsingHeader("Performance")) {
        ImGui::Text("FPS : %1f", currentFPS);
        ImGui::Text("FrameTime: %3f ms", 1000.0 / currentFPS);
        ImGui::Text("DrawCall: %d", DrawCallsCounter);
        ImGui::Text("Instancing: Incoming.");
    }

    if (ImGui::CollapsingHeader("User Settings")) {
        if (ImGui ::Combo("Light Model", &lightModeSelection, lightModeSelected, IM_ARRAYSIZE(lightModeSelected))) {
        }

        ImGui::SliderFloat("TimeScale", &userUpDown, 0.0, 1.0);
        ImGui::SliderFloat("User T", &userLeftRight, 0.0, 1.0);
    }

    if (ImGui::CollapsingHeader("Post Processing")) {
        ImGui::Checkbox("Enable Post Processing", &enablePostProcessing);
        ImGui::Checkbox("Invert Colors", &postFX.enableInvert);
        ImGui::Checkbox("Grayscale", &postFX.enableGrayscale);

        ImGui::Separator();

        ImGui::Checkbox("Chromatic Aberration", &postFX.enableChromaticAberration);
        if (postFX.enableChromaticAberration)
            ImGui::SliderFloat("Chroma Intensity", &postFX.chromaIntensity, 0.0f, 0.02f);

        ImGui::Separator();

        ImGui::Checkbox("Correct Gamma", &postFX.correctGamma);

        ImGui::Checkbox("Enable Kernel", &postFX.enableKernel);
        if (postFX.enableKernel) {
            const char* kernels[] = {"Blur", "Sharpen", "BoxBlur", "Emboss"};
            ImGui::Combo("Kernel Type", &postFX.kernelType, kernels, IM_ARRAYSIZE(kernels));
        }
    }

    if (ImGui::CollapsingHeader("Sun")) {
        ImGui::Checkbox("Update Sun", &updateSun);
        ImGui::SliderFloat("Sun Theta", &sunTheta, 0.0, glm::two_pi<float>());
        ImGui::SliderFloat("Sun Radius", &sunRadius, 1.0, 500.0);
        ImGui::SliderFloat("Sun Speed", &userSunSpeed, 0.0, 1.0f);
    }

    if (ImGui::CollapsingHeader("Meshes & Models")) {
        ImGui::Checkbox("Rotate Stuffs", &UI_rotateStuff);
        if (ImGui::Combo("Choose View Mode", &viewMode, viewModes, IM_ARRAYSIZE(viewModes))) {
            if (viewMode == 0) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else if (viewMode == 1) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
        ImGui ::Combo("Choose Object to Draw", &objectViewSelection, objectSelected, IM_ARRAYSIZE(objectSelected));
    }

    if (ImGui::CollapsingHeader("Lights")) {
        ImGui::SliderFloat("FlashLight Radius", &flashLightRadius, 1.0, 20.0f);
        ImGui::SliderFloat("Attenuation Linear ", &attenuation[1], 0.014, 0.7);
        ImGui::SliderFloat("Attenuation Quadratic ", &attenuation[2], 0.000007, 1.8);
        ImGui::ColorEdit3("Ambient Light Color", ambientLightColor);
        ImGui::ColorEdit3("Diffuse Light Color", diffuseLightColor);
        ImGui::ColorEdit3("Specular Light Color", specularLightColor);
    }

    if (ImGui::CollapsingHeader("Camera Settings")) {
        ImGui::SliderFloat("Camera Speed", &cam.MovementSpeed, Camera::MIN_SPEED, Camera::MAX_SPEED);
        if (ImGui ::Combo("Camera Mode", &cameraModeSelection, cameraModeSelected, IM_ARRAYSIZE(cameraModeSelected))) {
            cam.type = (CameraType)cameraModeSelection;
        }
        if (ImGui ::Combo("Projection", &projectionModeSelection, projectionModeSelected,
                          IM_ARRAYSIZE(projectionModeSelected))) {
        }
    }

    g_ImGuiConsole.Draw("Console");

    ImGui::End();

// -- UI Render
#pragma endregion
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
