#pragma once

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// clang-format on
#include <vector>

#include "../game/GameObject.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "Camera.h"
#include "Logger.h"
#include "Model.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "ShaderReloader.h"
#include "SlyMath.H"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "stb_image.h"

struct PostProcessSettings {
    bool enableChromaticAberration = false;
    float chromaIntensity = 0.02f;
    bool enableGrayscale;

    bool enableBlur = false;
    float blurRadius = 1.0f;

    bool enableInvert = false;

    bool enableKernel;
    int kernelType = 0;
};

struct TerrainData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int NUM_STRIPS;
    unsigned int NUM_VERTS_PER_STRIP;
};

// -- Prototypes
void GetInputs(GLFWwindow* window);
void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mode);
unsigned int LoadCubeMap(std::vector<std::string> facePaths);
TerrainData GetTerrainDataFromHeightMap(const char* filePath);

// -- Implemented --
inline float ElapsedTime() {
    return (float)glfwGetTime();
}
inline void LogMaxVertexAttributes() {
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    LOG("Max Vertex Attibutes", maxAttributes);
}

static inline int DrawCallsCounter = 0;

static void OpenGlDraw(GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
    DrawCallsCounter++;
}

enum class EngineState { ACTIVE, PAUSED };

class Engine {
   public:
    EngineState State;
    bool Keys[1024];
    unsigned int Width, Height;

    GameObject* Player;
    SpriteRenderer* Renderer;
    PostProcessSettings postFX;

    Engine(unsigned int width, unsigned int height);
    ~Engine();

    void Initialize();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render(float deltaTime);
    void Exit();

    void UpdatePostProcessFrameBuffer(int width, int height);
    void SetShaderLightsDatas(Shader& shader, glm::vec3 lightPos);

    const Texture* albedo;
    Texture* mask;
    Texture* diffuse;
    Texture* grass;
    Texture* specular;
    Texture* emissive;
    Model* backpackModel;
    Model* waterPlane;
    // Model cathedralModel("Resources/Models/sibenik/sibenik.obj");
    // Model villageModel("Resources/Models/rungholt/rungholt.obj");

    /*
    Shader phongShader("Shaders/vert.vs", "Shaders/frag.fs");
    Shader lightShader("Shaders/light_source_vertex.vs", "Shaders/light_source_frag.fs");
    Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
    Shader waterShader("Shaders/water.vs", "Shaders/water.fs");
    Shader grassShader("Shaders/grass_vert.vs", "Shaders/grass_frag.fs");
    Shader postProcessShader("Shaders/postprocess_vert.vs", "Shaders/postprocess_frag.fs");
    */

    //--Globals ? Todo : Classes ^ ^
    static inline unsigned int skyboxVAO, skyboxVBO;
    static inline GLuint terrainVAO, terrainVBO, terrainEBO;
    static inline unsigned int ndcQuadVAO, ndcQuadVBO;
    static inline unsigned int QuadVAO, QuadVBO;
    static inline unsigned int CubeVBO, CubeVAO;
    static inline unsigned int lightVAO;
    static inline unsigned int cubemapTexture;
    static inline std::vector<Model*> models;
    static inline std::vector<glm::vec3> vegetation;
    static inline unsigned int framebuffer;
    static inline std::vector<ShaderReloader*> reloaders;
    static inline auto lastCheck = std::chrono::steady_clock::now();
    static inline glm::highp_mat4 projectionMatrix;
    static inline double currentFPS = 0;
    static inline unsigned int renderTexture;
    static inline unsigned int rbo;
    static inline float userUpDown = 0.01;
    static inline float userLeftRight = 1.0;

    static inline glm::vec3 pointLightAmbient;
    static inline glm::vec3 pointLightDiffuse;
    static inline glm::vec3 pointLightSpecular;
    static inline glm::vec3 flashLightAmbient;
    static inline glm::vec3 flashLightDiffuse;
    static inline glm::vec3 flashLightSpecular;

    static inline glm::vec3 LightPosition;
    static inline Camera cam;
    static inline std::chrono::milliseconds checkInterval;

#pragma region UI EXPOSED
    // -- IMGUI EXPOSED

    inline static bool enablePostProcessing = false;
    static inline bool UI_rotateStuff = true;
    static inline float ambientLightColor[3] = {1.0, 1.0f, 1.0f};
    static inline float diffuseLightColor[3] = {0.8f, 0.8f, 0.8f};
    static inline float specularLightColor[3] = {0.0, 0.0f, 0.0f};
    static inline float attenuation[3] = {1.0, 0.09f, 0.032f};
    inline static const char* viewModes[]{"Normal", "Wireframe"};
    inline static const char* objectSelected[]{"Backpack", "Water", "Village", "Mercantile"};
    inline static const char* lightModeSelected[]{"Phong", "Toon", "PBR"};
    inline static const char* cameraModeSelected[]{"FreeCamera", "FPS"};
    inline static const char* projectionModeSelected[]{"Perspective", "Orthographic"};
    inline static const char* windWakerSelected[]{"Ariel",         "Crab",   "HelmarocKing", "Medoli",
                                                  "Phantom Sword", "Shield", "Valoo",        "Mercantile"};

    inline static float NearPlane = 0.1f;
    inline static float FarPlane = 5000.0f;
    inline static int viewMode = 0;                 // -- Lit Shaded
    inline static int objectViewSelection = 0;      // -- Backpack
    inline static int windWakerSelection = 7;       // -- Mercantile
    inline static int lightModeSelection = 0;       // -- Phong/Toon/PBR
    inline static int cameraModeSelection = 0;      // -- Free/FPS
    inline static int projectionModeSelection = 0;  // -- Perspective/Orthographic
    static inline float flashLightRadius = 10.0f;
    static inline bool updateSun = false;
    static inline float userSunSpeed = 0.01f;
    static inline float sunTheta = 10.0f;
    static inline float sunRadius = 500.0f;
#pragma endregion

#pragma region RawDatas

    static inline const float skyboxVertices[] = {  // positions
        -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

        1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

    static inline const float quadVertices[] = {
        // positions           // normals           // texCoords
        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right

        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f   // top-right
    };

    static inline const float NDCQuadVertices[] = {
        // vertex attributes for a quad that fills the
        // entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f};

    static inline const float cubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f};

    static inline const glm::vec3 cubePositions[] = {glm::vec3(2.0f, -1.0f, -1.0f),  glm::vec3(2.0f, 5.0f, -15.0f),
                                                     glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                                     glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                                     glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                                     glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    static inline const glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
                                                           glm::vec3(-4.0f, 2.0f, -12.0f),
                                                           glm::vec3(0.0f, 0.0f, -3.0f)};
#pragma endregion
};