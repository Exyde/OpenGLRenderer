#include "Engine.h"

// -- Player
const glm::vec2 PLAYER_SIZE(100.0F, 20.0f);
const float PLAYER_VELOCITY(500.0f);

GameObject* Player;
SpriteRenderer* Renderer;
const std::string PLAYER_TEXTURE = "player";

Engine::Engine(unsigned int width, unsigned int height)
    : State(EngineState::ACTIVE), Keys(), Width(width), Height(height) {}

Engine::~Engine() {}

void Engine::Initialize() {
    // -- Log
    LOG_INFO(LogCategory::Engine, "Initializing Engine Mode...");

    // -- Create Projection Matrix
    float w = static_cast<float>(this->Width);
    float h = static_cast<float>(this->Height);
    // Todo : projection

    // -- Load & Setup Main Shader
    ResourceLoader::LoadShader("Shaders/sprite.vs", "Shaders/sprite.fs", nullptr, "spriteShader");
    ResourceLoader::GetShader("spriteShader").Use();
    ResourceLoader::GetShader("spriteShader").SetInt("sprite", 0);
    // ResourceLoader::GetShader("spriteShader").SetMat4("projection", projection);

    // -- Setup Sprite Renderer
    Renderer = new SpriteRenderer(ResourceLoader::GetShader("spriteShader"));

    // -- Load Textures
    ResourceLoader::LoadTexture2D("Resources/Textures/paddle.png", true, PLAYER_TEXTURE);
    ResourceLoader::LoadTexture2D("Resources/Textures/awesomeface.png", true, "face");
    ResourceLoader::LoadTexture2D("Resources/Textures/block.png", false, "block");
    ResourceLoader::LoadTexture2D("Resources/Textures/block_solid.png", false, "block_solid");
    ResourceLoader::LoadTexture2D("Resources/Textures/background.jpg", false, "background");
    ResourceLoader::LoadTexture2D("Resources/Textures/skybox/back.png", false, "skybox");

    // -- Create Player
    glm::vec2 playerPos(this->Width / 2.0f - PLAYER_SIZE.x / 2.0F, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceLoader::GetTexture2D(PLAYER_TEXTURE));

    // -- INIT FROM MAIN

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

#pragma region CUBE AND LIGHTS VAOS VBOS

    // -- Skybox --
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // -- Terrain
    TerrainData terrainData = GetTerrainDataFromHeightMap("Resources/Textures/iceland_heightmap.png");
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);
    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, terrainData.vertices.size() * sizeof(float), &terrainData.vertices[0],
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainData.indices.size() * sizeof(unsigned int), &terrainData.indices[0],
                 GL_STATIC_DRAW);

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
#pragma region Texture Shaders Models

    ShaderReloader mainReloader(phongShader);
    ShaderReloader grassReloader(grassShader);
    ShaderReloader fullScreenReloader(postProcessShader);
    ShaderReloader skyboxReloader(skyboxShader);
    ShaderReloader waterReloader(waterShader);
    reloaders.push_back(&mainReloader);
    reloaders.push_back(&grassReloader);
    reloaders.push_back(&fullScreenReloader);
    reloaders.push_back(&waterReloader);

    std::vector<std::string> cubemapPaths{
        "Resources/Textures/skybox/right.png", "Resources/Textures/skybox/left.png",
        "Resources/Textures/skybox/top.png",   "Resources/Textures/skybox/bottom.png",
        "Resources/Textures/skybox/front.png", "Resources/Textures/skybox/back.png",
    };

    cubemapTexture = LoadCubeMap(cubemapPaths);

    skyboxShader.Use();
    skyboxShader.SetInt("skybox", 0);

    models.push_back(backpackModel);
    models.push_back(waterPlane);

#ifdef WIND_WAKER
    std::vector<Model> wwModels;
    wwModels.push_back(Model("Resources/Models/WindWaker/Ariel/lshand.dae"));
    wwModels.push_back(Model("Resources/Models/WindWaker/Crab/Crab.dae"));
    wwModels.push_back(
        Model("Resources/Models/WindWaker/HelmarocKing/"
              "HelmarocKingMask.obj"));
    wwModels.push_back(Model("Resources/Models/WindWaker/Medoli/MedliWithHarp.dae"));
    wwModels.push_back(Model("Resources/Models/WindWaker/PhantomSword/pg_sword.obj"));
    wwModels.push_back(Model("Resources/Models/WindWaker/Shield/Shield.obj"));
    wwModels.push_back(Model("Resources/Models/WindWaker/Valoo/dr.dae"));
    wwModels.push_back(Model("Resources/Models/WindWaker/Windfall/Windfall.obj"));
#endif

    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

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

    // -- Log
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

    // -- Check Shader Reloaders
    auto now = std::chrono::steady_clock::now();
    if (now - lastCheck > checkInterval) {
        lastCheck = now;
        for (ShaderReloader* r : reloaders) {
            r->CheckForChanges();
        }
    }
}

void Engine::Render(float deltaTime) {
    if (this->State == EngineState::ACTIVE) {
        Renderer->DrawSprite(ResourceLoader::GetTexture2D("skybox"), glm::vec2(0.0f, 0.0f),
                             glm::vec2(this->Width, this->Height), 0.0f);

        DrawCallsCounter = 0;

#pragma region SunUpdate
        // -- Transformed positions
        float sunSpeed = deltaTime * userSunSpeed;
        if (updateSun)
            sunTheta += sunSpeed;
        glm::vec3 sunYaw = glm::vec3(cos(sunTheta) * sunRadius, 0, sin(sunTheta) * sunRadius);
        glm::vec3 sunPitch = glm::vec3(0, cos(sunTheta) * sunRadius, sin(sunTheta) * sunRadius);
        auto offsetedLightPos = LightPosition.GLM() + sunYaw + sunPitch;
#pragma endregion
#pragma region ImGui Begin
        // -- UI IMGUI
        // 1. New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
#pragma endregion
#pragma region FIRST PASS

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
            projectionMatrix =
                glm::perspective(glm::radians(cam.Fov), (float)Width / (float)Height, NearPlane, FarPlane);
        } else {
            projectionMatrix = glm::ortho(0.0f, (float)Width, 0.0f, (float)Height, -1.0f, 1.0f);
        }
        glm::mat4 viewMatrix = cam.GetViewMatrix();
        glm::mat4 modelMatrix = glm::mat4(1.0F);

        // -- Textures Units ?
        glActiveTexture(GL_TEXTURE0);
        diffuse.Bind();
        glActiveTexture(GL_TEXTURE1);
        specular.Bind();
        glActiveTexture(GL_TEXTURE2);
        emissive.Bind();

        // -- Phong Shader
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

        //-- DrawGround
        modelMatrix = glm::mat4(1.0F);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10, 10, 10));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1, 0));
        phongShader.SetMat4("model", modelMatrix);
        glBindVertexArray(CubeVAO);
        OpenGlDraw(GL_TRIANGLES, 0, 36);

        /*
        //-- Draw Terrain
        glBindVertexArray(terrainVAO);
        bool drawTerrain = false;
        if (drawTerrain) {
            for (unsigned int strip = 0; strip < terrainData.NUM_STRIPS; ++strip) {
                glDrawElements(GL_TRIANGLE_STRIP, terrainData.NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT,
                               (void*)(sizeof(unsigned int) * terrainData.NUM_VERTS_PER_STRIP * strip));
            }
        }
        */

        // -- Multiples Floating Cubes
        glBindVertexArray(CubeVAO);
        for (unsigned int i = 0; i < 10; i++) {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            float angle = 20.0f * i;
            if (UI_rotateStuff) {
                modelMatrix =
                    glm::rotate(modelMatrix, glm::radians(angle + ElapsedTime() * 100), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            phongShader.SetMat4("model", modelMatrix);

            OpenGlDraw(GL_POINTS, 0, 36);
        }

        modelMatrix = glm::mat4(1.0F);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        phongShader.SetMat4("model", modelMatrix);

        models[objectViewSelection].Draw(phongShader);

#ifdef WIND_WAKER
        wwModels[windWakerSelection].Draw(phongShader);
#endif

        waterShader.Use();
        modelMatrix = glm::mat4(1.0F);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1000.0f));
        waterShader.SetMat4("model", modelMatrix);
        waterShader.SetMat4("view", viewMatrix);
        waterShader.SetMat4("projection", projectionMatrix);
        waterShader.SetFloat("uTime", ElapsedTime() * userUpDown);
        // -- Water
        waterPlane.Draw(waterShader);

        // -- Grass
        grassShader.Use();
        glActiveTexture(GL_TEXTURE0);
        grass.Bind();
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

        // -- Light Object -- //
        lightShader.Use();
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, offsetedLightPos);
        lightModel = glm::scale(lightModel, glm::vec3(25.0f));
        lightShader.SetMat4("model", lightModel);
        lightShader.SetMat4("view", viewMatrix);
        lightShader.SetMat4("projection", projectionMatrix);
        lightShader.SetVec3("LightPos", offsetedLightPos);
        lightShader.SetVec3("LightColor",
                            Vector3(diffuseLightColor[0], diffuseLightColor[1], diffuseLightColor[2]).GLM());
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

        // -- Skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glm::mat4 skyboxViewWithoutTranslation = glm::mat4(glm::mat3(cam.GetViewMatrix()));
        skyboxShader.SetMat4("view", skyboxViewWithoutTranslation);
        skyboxShader.SetMat4("projection", projectionMatrix);
        OpenGlDraw(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

#pragma endregion

#pragma region SECOND PASS POST PROCESS

        if (enablePostProcessing) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(1.0f, 1.0F, 1.0F, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT);

            postProcessShader.Use();
            postProcessShader.SetFloat("uTime", ElapsedTime() * userUpDown);
            postProcessShader.SetBool("uEnableChroma", postFX.enableChromaticAberration);
            postProcessShader.SetFloat("uChromaIntensity", postFX.chromaIntensity);
            postProcessShader.SetBool("uEnableInvert", postFX.enableInvert);
            postProcessShader.SetBool("uEnableGrayscale", postFX.enableGrayscale);
            postProcessShader.SetBool("uEnableKernel", postFX.enableKernel);
            postProcessShader.SetInt("uKernelType", postFX.kernelType);
            glBindVertexArray(ndcQuadVAO);
            glDisable(GL_DEPTH_TEST);
            glBindTexture(GL_TEXTURE_2D, renderTexture);
            OpenGlDraw(GL_TRIANGLES, 0, 6);
        }

#pragma endregion

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

            ImGui ::Combo("Choose Wind Waker Asset", &windWakerSelection, windWakerSelected,
                          IM_ARRAYSIZE(windWakerSelected));
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
            if (ImGui ::Combo("Camera Mode", &cameraModeSelection, cameraModeSelected,
                              IM_ARRAYSIZE(cameraModeSelected))) {
                cam.type = (CameraType)cameraModeSelection;
            }
            if (ImGui ::Combo("Projection", &projectionModeSelection, projectionModeSelected,
                              IM_ARRAYSIZE(projectionModeSelected))) {
            }
        }

        ImGui::End();

// -- UI Render
#pragma endregion
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void Engine::Exit() {
    glDeleteFramebuffers(1, &framebuffer);
    ResourceLoader::Clear();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

TerrainData GetTerrainDataFromHeightMap(const char* filePath) {
    TerrainData terrainData;

    int width, height, channels;
    unsigned short* data16 = stbi_load_16(filePath, &width, &height, &channels, 0);

    if (data16) {
        LOG(" [Terrain] - LoadHeightmap at", filePath, " -- W: ", width, " --H: ", height, " --Channels: ", channels);
    } else {
        LOG_ERROR(LogCategory::Engine, "Failed to load heightmap data:", filePath);
    }

    std::vector<float> vertices;
    float yScale = 50.0f;
    float yShift = 0.0F;

    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            // -- Data 16
            unsigned short value16 = data16[width * i + j];
            float h = (float)value16 / 65535.0F;

            float worldX = (i - width / 2.0F);
            float worldZ = (j - height / 2.0F);
            float worldY = h * yScale - yShift;

            vertices.push_back(worldX);
            vertices.push_back(worldY);
            vertices.push_back(worldZ);
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < height - 1; i++) {
        for (unsigned int j = 0; j < width; j++) {
            for (unsigned int k = 0; k < 2; k++)  // for each side of the strip
            {
                indices.push_back(j + width * (i + k));
            }
        }
    }

    terrainData.vertices = vertices;
    terrainData.indices = indices;
    terrainData.NUM_STRIPS = height - 1;
    terrainData.NUM_VERTS_PER_STRIP = width * 2;

    stbi_image_free(data16);

    return terrainData;
}

void SetShaderLightsDatas(Shader& shader, glm::vec3 lightPos) {
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