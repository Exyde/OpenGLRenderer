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

// --  Globals ? Todo : Classes ^^
unsigned int skyboxVAO, skyboxVBO;
GLuint terrainVAO, terrainVBO, terrainEBO;
unsigned int ndcQuadVAO, ndcQuadVBO;
unsigned int QuadVAO, QuadVBO;
unsigned int CubeVBO, CubeVAO;
unsigned int lightVAO;
unsigned int cubemapTexture;
std::vector<Model> models;
std::vector<glm::vec3> vegetation;
unsigned int framebuffer;
std::vector<ShaderReloader*> reloaders;
auto lastCheck = std::chrono::steady_clock::now();
const std::chrono::milliseconds checkInterval(500);
glm::highp_mat4 projectionMatrix;
static double currentFPS = 0;

Texture albedo("Resources/Textures/container.jpg", GL_CLAMP_TO_BORDER, false, "diffuse");
Texture mask("Resources/Textures/awesomeface.png", GL_CLAMP_TO_BORDER, false, "diffuse");
Texture diffuse("Resources/Textures/container_diffuse.png", GL_CLAMP_TO_BORDER, true, "diffuse");
Texture cat("Resources/Textures/cat.png", GL_CLAMP_TO_BORDER, false, "diffuse");
Texture grass("Resources/Textures/grass.png", GL_CLAMP_TO_EDGE, true, "diffuse");
Texture specular("Resources/Textures/container_specular.png", GL_CLAMP_TO_BORDER, true, "specular");
Texture emissive("Resources/Textures/container_emmisive.jpg", GL_CLAMP_TO_BORDER, false, "diffuse");
Model backpackModel("Resources/Models/backpack/backpack.obj");
Model waterPlane("Resources/Models//Water/waterplane.obj");
// Model cathedralModel("Resources/Models/sibenik/sibenik.obj");
// Model villageModel("Resources/Models/rungholt/rungholt.obj");

Shader lightShader("Shaders/light_source_vertex.vs", "Shaders/light_source_frag.fs");
Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
Shader phongShader("Shaders/vert.vs", "Shaders/frag.fs");
Shader waterShader("Shaders/water.vs", "Shaders/water.fs");
Shader grassShader("Shaders/grass_vert.vs", "Shaders/grass_frag.fs");
Shader postProcessShader("Shaders/postprocess_vert.vs", "Shaders/postprocess_frag.fs");

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

constexpr glm::vec4 ColorWhite = glm::vec4(1.0);
constexpr glm::vec4 ColorBlack = glm::vec4(0.0);
constexpr glm::vec4 ColorGray = glm::vec4(0.5);
unsigned int renderTexture;
unsigned int rbo;
bool cursorVisible = false;
float userUpDown = 0.01;
float userLeftRight = 1.0;
Vector3 LightPosition(-2.0f, 1.5F, 3.3F);
Camera cam(Vector3(0.0f, 0.0f, 0.0f));

// -- Prototypes
void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mode);
unsigned int LoadCubeMap(std::vector<std::string> facePaths);
void GetInputs(GLFWwindow* window);
void SetShaderLightsDatas(Shader& shader, glm::vec3 lightPos);
TerrainData GetTerrainDataFromHeightMap(const char* filePath);

// -- Implemented --
float ElapsedTime() {
    return (float)glfwGetTime();
}
void LogMaxVertexAttributes() {
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    LOG("Max Vertex Attibutes", maxAttributes);
}

static inline int DrawCallsCounter = 0;

static void OpenGlDraw(GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
    DrawCallsCounter++;
}

#pragma region RawDatas

float skyboxVertices[] = {  // positions
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

float quadVertices[] = {
    // positions           // normals           // texCoords
    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
    0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right

    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
    0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
    0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f   // top-right
};

float NDCQuadVertices[] = {  // vertex attributes for a quad that fills the
                             // entire screen in Normalized Device Coordinates.
                             // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f};

float cubeVertices[] = {
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

glm::vec3 cubePositions[] = {glm::vec3(2.0f, -1.0f, -1.0f),  glm::vec3(2.0f, 5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                             glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                             glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
                                   glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};
#pragma endregion

#pragma region UI EXPOSED
// -- IMGUI EXPOSED

static bool enablePostProcessing = false;
bool UI_rotateStuff = true;
float ambientLightColor[3] = {1.0, 1.0f, 1.0f};
float diffuseLightColor[3] = {0.8f, 0.8f, 0.8f};
float specularLightColor[3] = {0.0, 0.0f, 0.0f};
float attenuation[3] = {1.0, 0.09f, 0.032f};
static const char* viewModes[]{"Normal", "Wireframe"};
static const char* objectSelected[]{"Backpack", "Water", "Village", "Mercantile"};
static const char* lightModeSelected[]{"Phong", "Toon", "PBR"};
static const char* cameraModeSelected[]{"FreeCamera", "FPS"};
static const char* projectionModeSelected[]{"Perspective", "Orthographic"};
static const char* windWakerSelected[]{"Ariel",         "Crab",   "HelmarocKing", "Medoli",
                                       "Phantom Sword", "Shield", "Valoo",        "Mercantile"};

static float NearPlane = 0.1f;
static float FarPlane = 5000.0f;
static int viewMode = 0;                 // -- Lit Shaded
static int objectViewSelection = 4;      // -- Backpack
static int windWakerSelection = 7;       // -- Mercantile
static int lightModeSelection = 0;       // -- Phong/Toon/PBR
static int cameraModeSelection = 0;      // -- Free/FPS
static int projectionModeSelection = 0;  // -- Perspective/Orthographic
glm::vec3 pointLightAmbient(0.05f);
glm::vec3 pointLightDiffuse(0.0f, 0.2f, 0.7f);
glm::vec3 pointLightSpecular(1.0f);

glm::vec3 flashLightAmbient(0.05f);
glm::vec3 flashLightDiffuse(0.8f, 0.2f, 0.6f);
glm::vec3 flashLightSpecular(1.0f);
float flashLightRadius = 10.0f;
bool updateSun = false;
float userSunSpeed = 0.01f;
float sunTheta = 10.0f;
float sunRadius = 500.0f;
#pragma endregion

enum class EngineState { ACTIVE, PAUSED };
PostProcessSettings postFX;

class Engine {
   public:
    EngineState State;
    bool Keys[1024];
    unsigned int Width, Height;

    Engine(unsigned int width, unsigned int height);
    ~Engine();

    void Initialize();
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render(float deltaTime);
    void Exit();
};