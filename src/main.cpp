// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// clang-format on

#include "main.h"

#include <cmath>
#include <iostream>
#include <thread>

#include "engine/Camera.h"
#include "engine/Model.h"
#include "engine/ResourceLoader.h"
#include "engine/Shader.h"
#include "engine/ShaderReloader.h"
#include "engine/SlyMath.H"
#include "engine/Texture.h"
#include "engine/stb_image.h"
#include "game/Game.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

enum class ApplicationMode { Engine, Game };

ApplicationMode appMode = ApplicationMode::Game;

#pragma region UserShitToMoveLaterAwayFromHere
const uint32_t SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
uint32_t CURRENT_WIDTH = 800, CURRENT_HEIGHT = 600;
unsigned int renderTexture;
unsigned int rbo;
bool cursorVisible = false;
float userUpDown = 0.01;
float userLeftRight = 1.0;
Vector3 LightPosition(-2.0f, 1.5F, 3.3F);
Camera cam(Vector3(0.0f, 0.0f, 0.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool FIRST_MOUSE = true;
float lastMouseX = 400;
float lastMouseY = 300;
#pragma endregion

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode);

constexpr glm::vec4 ColorWhite = glm::vec4(1.0);
constexpr glm::vec4 ColorBlack = glm::vec4(0.0);
constexpr glm::vec4 ColorGray = glm::vec4(0.5);

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

PostProcessSettings postFX;

#pragma region UI EXPOSED
// -- IMGUI EXPOSED

static int DrawCallsCounter = 0;
bool enablePostProcessing = false;
bool UI_rotateStuff = true;
float ambientLightColor[3] = {1.0, 1.0f, 1.0f};
float diffuseLightColor[3] = {0.8f, 0.8f, 0.8f};
float specularLightColor[3] = {0.0, 0.0f, 0.0f};
float attenuation[3] = {1.0, 0.09f, 0.032f};
static const char* viewModes[]{"Normal", "Wireframe"};
static const char* objectSelected[]{"Backpack", "Water", "Village",
                                    "Mercantile"};
static const char* lightModeSelected[]{"Phong", "Toon", "PBR"};
static const char* cameraModeSelected[]{"FreeCamera", "FPS"};
static const char* projectionModeSelected[]{"Perspective", "Orthographic"};
static const char* windWakerSelected[]{
    "Ariel",         "Crab",   "HelmarocKing", "Medoli",
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

// -- Raws Datas
#pragma region RawDatas

float skyboxVertices[] = {
    // positions
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
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

glm::vec3 cubePositions[] = {
    glm::vec3(2.0f, -1.0f, -1.0f),  glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};
#pragma endregion

unsigned int LoadCubeMap(std::vector<std::string> facePaths);

static void OpenGlDraw(GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
    DrawCallsCounter++;
}

void ToggleDepthBuffer(GLboolean state) {
    glDepthMask(state);
}  // -- Enable read-only depth buffer (fragment don't write to ZBuffer when
   // enabled)
float ElapsedTime() { return (float)glfwGetTime(); }
void ExitEngine() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}
void LogMaxVertexAttributes() {
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    LOG("Max Vertex Attibutes", maxAttributes);
}

#pragma region CallbackOpenGL
void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height) {
    CURRENT_WIDTH = width;
    CURRENT_HEIGHT = height;
    glViewport(0, 0, width, height);

    if (appMode == ApplicationMode::Game) return;

    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (appMode == ApplicationMode::Game) return;

    if (cursorVisible) return;
    // cam.ProcessMouseScroll(yoffset);
    cam.MovementSpeed += (float)yoffset;
}

void Mouse_Callback(GLFWwindow* window, double xposin, double yposin) {
    if (appMode == ApplicationMode::Game) return;

    if (cursorVisible) return;
    float xpos = static_cast<float>(xposin);
    float ypos = static_cast<float>(yposin);

    //-- avoid first frame snapping (should have a bool for first frame ?)
    if (FIRST_MOUSE) {
        FIRST_MOUSE = false;
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
    float deltaX = xpos - lastMouseX;
    float deltaY = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;

    cam.ProcessMouseMovement(deltaX, deltaY, true);
}
#pragma endregion

#pragma region Init
bool InitializeGLFW() {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    LOG("Succesfully Initialized GLFW");
    return true;
}

GLFWwindow* CreateWindow(uint32_t width, uint32_t height) {
    LOG("Creating main window");

    GLFWwindow* window =
        glfwCreateWindow(width, height, "Open GL Renderer v0.0.1", NULL, NULL);
    return window;
}

void InitIMGUI(GLFWwindow* window) {
    LOG("Initializing ImGUI");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
#pragma endregion

void GetInputs(GLFWwindow* window);
void SetShaderLightsDatas(Shader& shader, glm::vec3 lightPos);
TerrainData GetTerrainDataFromHeightMap(const char* filePath);

int main() {
#pragma region Init
    if (!InitializeGLFW()) {
        LOG_ERROR(LogCategory::Engine, "Failed to initialize GLFW");
        return -1;
    }

    GLFWwindow* window = CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    if (window == NULL) {
        LOG_ERROR(LogCategory::Engine, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // -- Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR(LogCategory::Engine, "Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    } else {
        LOG("Succesfully Initialized GLAD");
    }

    // -- Viewport dimensions ---
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // -- Setup --
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    InitIMGUI(window);
    stbi_set_flip_vertically_on_load(false);

    // -- Callback
    glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);
    glfwSetScrollCallback(window, Scroll_Callback);

    if (appMode == ApplicationMode::Game) {
        LOG("Initalizing Game Mode");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glfwSetKeyCallback(window, key_callback);

        game.Initialize();
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        while (!glfwWindowShouldClose(window)) {
            // -- Delta time
            float currentFrame = ElapsedTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            glfwPollEvents();

            game.ProcessInput(deltaTime);
            game.Update(deltaTime);
            game.Render();

            glfwSwapBuffers(window);
        }

        ResourceLoader::Clear();
        glfwTerminate();
        return 0;
    }

    if (appMode == ApplicationMode::Engine) {
        LOG("Initalizing Engine Mode");

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
#pragma endregion

#pragma region CUBE AND LIGHTS VAOS VBOS

        // -- Skybox --
        unsigned int skyboxVAO, skyboxVBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void*)0);

        // -- Terrain
        TerrainData terrainData = GetTerrainDataFromHeightMap(
            "Resources/Textures/iceland_heightmap.png");
        GLuint terrainVAO, terrainVBO, terrainEBO;
        glGenVertexArrays(1, &terrainVAO);
        glBindVertexArray(terrainVAO);
        glGenBuffers(1, &terrainVBO);
        glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     terrainData.vertices.size() * sizeof(float),
                     &terrainData.vertices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &terrainEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     terrainData.indices.size() * sizeof(unsigned int),
                     &terrainData.indices[0], GL_STATIC_DRAW);

        // -- NDC Quad
        unsigned int ndcQuadVAO, ndcQuadVBO;
        glGenVertexArrays(1, &ndcQuadVAO);
        glGenBuffers(1, &ndcQuadVBO);
        glBindVertexArray(ndcQuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, ndcQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(NDCQuadVertices), &NDCQuadVertices,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void*)(2 * sizeof(float)));

        // -- Quad Datas
        unsigned int QuadVAO, QuadVBO;
        glGenVertexArrays(1, &QuadVAO);
        glGenBuffers(1, &QuadVBO);
        glBindVertexArray(QuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // -- Cube Datas ---
        unsigned int CubeVBO, CubeVAO;
        glGenVertexArrays(1, &CubeVAO);
        glGenBuffers(1, &CubeVBO);
        glBindVertexArray(CubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // -- Light Data
        unsigned int lightVAO;
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(0);

#pragma endregion
#pragma region Texture Shaders Models

        Shader lightShader("Shaders/light_source_vertex.vs",
                           "Shaders/light_source_frag.fs");
        Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
        Shader phongShader("Shaders/vert.vs", "Shaders/frag.fs");
        Shader waterShader("Shaders/water.vs", "Shaders/water.fs");
        Shader grassShader("Shaders/grass_vert.vs", "Shaders/grass_frag.fs");
        Shader postProcessShader("Shaders/postprocess_vert.vs",
                                 "Shaders/postprocess_frag.fs");

        ShaderReloader mainReloader(phongShader);
        ShaderReloader grassReloader(grassShader);
        ShaderReloader fullScreenReloader(postProcessShader);
        ShaderReloader skyboxReloader(skyboxShader);
        ShaderReloader waterReloader(waterShader);

        std::vector<ShaderReloader> reloaders{mainReloader, grassReloader,
                                              fullScreenReloader,
                                              skyboxReloader, waterReloader};

        std::vector<std::string> cubemapPaths{
            "Resources/Textures/skybox/right.png",
            "Resources/Textures/skybox/left.png",
            "Resources/Textures/skybox/top.png",
            "Resources/Textures/skybox/bottom.png",
            "Resources/Textures/skybox/front.png",
            "Resources/Textures/skybox/back.png",
        };
        unsigned int cubemapTexture = LoadCubeMap(cubemapPaths);

        skyboxShader.Use();
        skyboxShader.SetInt("skybox", 0);

        Texture albedo("Resources/Textures/container.jpg", GL_CLAMP_TO_BORDER,
                       false, "diffuse");
        Texture mask("Resources/Textures/awesomeface.png", GL_CLAMP_TO_BORDER,
                     false, "diffuse");
        Texture diffuse("Resources/Textures/container_diffuse.png",
                        GL_CLAMP_TO_BORDER, true, "diffuse");
        Texture cat("Resources/Textures/cat.png", GL_CLAMP_TO_BORDER, false,
                    "diffuse");
        Texture grass("Resources/Textures/grass.png", GL_CLAMP_TO_EDGE, true,
                      "diffuse");
        Texture specular("Resources/Textures/container_specular.png",
                         GL_CLAMP_TO_BORDER, true, "specular");
        Texture emissive("Resources/Textures/container_emmisive.jpg",
                         GL_CLAMP_TO_BORDER, false, "diffuse");

        Model backpackModel("Resources/Models/backpack/backpack.obj");
        Model waterPlane("Resources/Models//Water/waterplane.obj");
        // Model cathedralModel("Resources/Models/sibenik/sibenik.obj");
        // Model villageModel("Resources/Models/rungholt/rungholt.obj");

        std::vector<Model> models{backpackModel, waterPlane, backpackModel,
                                  backpackModel, backpackModel};

#ifdef WIND_WAKER
        std::vector<Model> wwModels;
        wwModels.push_back(
            Model("Resources/Models/WindWaker/Ariel/lshand.dae"));
        wwModels.push_back(Model("Resources/Models/WindWaker/Crab/Crab.dae"));
        wwModels.push_back(
            Model("Resources/Models/WindWaker/HelmarocKing/"
                  "HelmarocKingMask.obj"));
        wwModels.push_back(
            Model("Resources/Models/WindWaker/Medoli/MedliWithHarp.dae"));
        wwModels.push_back(
            Model("Resources/Models/WindWaker/PhantomSword/pg_sword.obj"));
        wwModels.push_back(
            Model("Resources/Models/WindWaker/Shield/Shield.obj"));
        wwModels.push_back(Model("Resources/Models/WindWaker/Valoo/dr.dae"));
        wwModels.push_back(
            Model("Resources/Models/WindWaker/Windfall/Windfall.obj"));
#endif

        std::vector<glm::vec3> vegetation;
        vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
        vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
        vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
        vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
        vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

        // -- FrameBuffers

        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &renderTexture);
        glBindTexture(GL_TEXTURE_2D, renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, renderTexture, 0);
        // -- You could also attach a depth-stencil buffer to this
        // framebuffer with a 32 bit texture (24 bit for depth, 8 for
        // Stencil)
        // -- Use a RenderBufferObject for Depth & Stencil as Read it's read
        // only
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                              SCREEN_WIDTH, SCREEN_HEIGHT);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, rbo);

        // -- To be valid, attach at least on buffer, one color attachment,
        // all attachements should be complete, each buffer same samples
        // count.
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE) {
            LOG_ERROR(LogCategory::Engine, "FrameBuffer is not complete !");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto lastCheck = std::chrono::steady_clock::now();
        const std::chrono::milliseconds checkInterval(500);

#pragma endregion

        // -- Render Loop
        while (!glfwWindowShouldClose(window)) {
            // -- Skip everything if windows not focused.
            if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                glfwPollEvents();
                continue;
            }

            // -- Input Handling
            GetInputs(window);
            DrawCallsCounter = 0;

            // -- Update Shader Timer
            auto now = std::chrono::steady_clock::now();
            if (now - lastCheck > checkInterval) {
                lastCheck = now;
                for (ShaderReloader r : reloaders) {
                    r.CheckForChanges();
                }
            }

            // -- Time Update
            float currentFrame = ElapsedTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // -- FPS Computation --
            static double lastTime = glfwGetTime();
            static int frameCount = 0;
            static double currentFPS = 0;
            double currentTime = glfwGetTime();
            frameCount++;

            if (currentTime - lastTime >= 1.0) {
                double fps = double(frameCount) / (currentTime - lastTime);
                frameCount = 0;
                lastTime = currentTime;

                currentFPS = fps;
            }

#pragma region SunUpdate
            // -- Transformed positions
            float sunSpeed = deltaTime * userSunSpeed;
            if (updateSun) sunTheta += sunSpeed;
            glm::vec3 sunYaw = glm::vec3(cos(sunTheta) * sunRadius, 0,
                                         sin(sunTheta) * sunRadius);
            glm::vec3 sunPitch = glm::vec3(0, cos(sunTheta) * sunRadius,
                                           sin(sunTheta) * sunRadius);
            auto offsetedLightPos = LightPosition.GLM() + sunYaw + sunPitch;
#pragma endregion

            // -- UI IMGUI
            // 1. New Frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport(
                0, ImGui::GetMainViewport(),
                ImGuiDockNodeFlags_PassthruCentralNode);

#pragma region FIRST PASS

            //-- FIRST PASS -- NORMAL SCENE BUT OF SCREEN RENDERING (Well
            // that's
            // the
            // way to Deferred I guess)
            if (enablePostProcessing) {
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            } else {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            glClearColor(0.2f, 0.3f, 0.3f, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                    GL_STENCIL_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            // here FOV, aspect ratio, near and far plane for perspective (w
            // scaled)

            glm::highp_mat4 projectionMatrix;
            if (projectionModeSelection == 0) {
                projectionMatrix = glm::perspective(
                    glm::radians(cam.Fov),
                    (float)CURRENT_WIDTH / (float)CURRENT_HEIGHT, NearPlane,
                    FarPlane);
            } else {
                projectionMatrix =
                    glm::ortho(0.0f, (float)CURRENT_WIDTH, 0.0f,
                               (float)CURRENT_HEIGHT, -1.0f, 1.0f);
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

            //-- Draw Terrain
            glBindVertexArray(terrainVAO);
            bool drawTerrain = false;
            if (drawTerrain) {
                for (unsigned int strip = 0; strip < terrainData.NUM_STRIPS;
                     ++strip) {
                    glDrawElements(
                        GL_TRIANGLE_STRIP, terrainData.NUM_VERTS_PER_STRIP,
                        GL_UNSIGNED_INT,
                        (void*)(sizeof(unsigned int) *
                                terrainData.NUM_VERTS_PER_STRIP * strip));
                }
            }

            // -- Multiples Floating Cubes
            glBindVertexArray(CubeVAO);
            for (unsigned int i = 0; i < 10; i++) {
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
                float angle = 20.0f * i;
                if (UI_rotateStuff) {
                    modelMatrix = glm::rotate(
                        modelMatrix, glm::radians(angle + ElapsedTime() * 100),
                        glm::vec3(1.0f, 0.3f, 0.5f));
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
                    float angle =
                        (double)glm::two_pi<float>() / grassCount * rot;
                    model =
                        glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
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
            lightShader.SetVec3("LightColor", Vector3(diffuseLightColor[0],
                                                      diffuseLightColor[1],
                                                      diffuseLightColor[2])
                                                  .GLM());
            glBindVertexArray(lightVAO);
            OpenGlDraw(GL_TRIANGLES, 0, 36);

            for (int i = 0; i < 4; i++) {
                lightModel = glm::mat4(1.0F);
                lightModel = glm::translate(lightModel, pointLightPositions[i]);
                lightModel = glm::scale(lightModel, glm::vec3(0.2f));
                lightShader.SetMat4("model", lightModel);
                lightShader.SetVec3("LightPos", pointLightPositions[i]);
                lightShader.SetVec3("LightColor", Vector3(pointLightDiffuse[0],
                                                          pointLightDiffuse[1],
                                                          pointLightDiffuse[2])
                                                      .GLM());

                OpenGlDraw(GL_TRIANGLES, 0, 36);
            }

            // -- Skybox
            glDepthFunc(GL_LEQUAL);
            skyboxShader.Use();
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glm::mat4 skyboxViewWithoutTranslation =
                glm::mat4(glm::mat3(cam.GetViewMatrix()));
            skyboxShader.SetMat4("view", skyboxViewWithoutTranslation);
            skyboxShader.SetMat4("projection", projectionMatrix);
            OpenGlDraw(GL_TRIANGLES, 0, 36);

            glDepthFunc(GL_LESS);

#pragma endregion

#pragma region SECOND PASS POST PROCESS

            if (enablePostProcessing) {
                // -- SECOND PASS FOR FULL SCREEN POST PROCESSING
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClearColor(1.0f, 1.0F, 1.0F, 1.0F);
                glClear(GL_COLOR_BUFFER_BIT);

                postProcessShader.Use();
                postProcessShader.SetFloat("uTime", ElapsedTime() * userUpDown);
                postProcessShader.SetBool("uEnableChroma",
                                          postFX.enableChromaticAberration);
                postProcessShader.SetFloat("uChromaIntensity",
                                           postFX.chromaIntensity);
                postProcessShader.SetBool("uEnableInvert", postFX.enableInvert);
                postProcessShader.SetBool("uEnableGrayscale",
                                          postFX.enableGrayscale);
                postProcessShader.SetBool("uEnableKernel", postFX.enableKernel);
                postProcessShader.SetInt("uKernelType", postFX.kernelType);
                glBindVertexArray(ndcQuadVAO);
                glDisable(GL_DEPTH_TEST);
                glBindTexture(GL_TEXTURE_2D, renderTexture);
                OpenGlDraw(GL_TRIANGLES, 0, 6);
            }

#pragma endregion

#pragma region IMGUI
            // -- IMGUI FOLLOW UP

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
                if (ImGui ::Combo("Light Model", &lightModeSelection,
                                  lightModeSelected,
                                  IM_ARRAYSIZE(lightModeSelected))) {
                }

                ImGui::SliderFloat("TimeScale", &userUpDown, 0.0, 1.0);
                ImGui::SliderFloat("User T", &userLeftRight, 0.0, 1.0);
            }

            if (ImGui::CollapsingHeader("Post Processing")) {
                ImGui::Checkbox("Enable Post Processing",
                                &enablePostProcessing);
                ImGui::Checkbox("Invert Colors", &postFX.enableInvert);
                ImGui::Checkbox("Grayscale", &postFX.enableGrayscale);

                ImGui::Separator();

                ImGui::Checkbox("Chromatic Aberration",
                                &postFX.enableChromaticAberration);
                if (postFX.enableChromaticAberration)
                    ImGui::SliderFloat("Chroma Intensity",
                                       &postFX.chromaIntensity, 0.0f, 0.02f);

                ImGui::Separator();

                ImGui::Checkbox("Enable Kernel", &postFX.enableKernel);
                if (postFX.enableKernel) {
                    const char* kernels[] = {"Blur", "Sharpen", "BoxBlur",
                                             "Emboss"};
                    ImGui::Combo("Kernel Type", &postFX.kernelType, kernels,
                                 IM_ARRAYSIZE(kernels));
                }
            }

            if (ImGui::CollapsingHeader("Sun")) {
                ImGui::Checkbox("Update Sun", &updateSun);
                ImGui::SliderFloat("Sun Theta", &sunTheta, 0.0,
                                   glm::two_pi<float>());
                ImGui::SliderFloat("Sun Radius", &sunRadius, 1.0, 500.0);
                ImGui::SliderFloat("Sun Speed", &userSunSpeed, 0.0, 1.0f);
            }

            if (ImGui::CollapsingHeader("Meshes & Models")) {
                ImGui::Checkbox("Rotate Stuffs", &UI_rotateStuff);
                if (ImGui::Combo("Choose View Mode", &viewMode, viewModes,
                                 IM_ARRAYSIZE(viewModes))) {
                    if (viewMode == 0) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    } else if (viewMode == 1) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    }
                }
                ImGui ::Combo("Choose Object to Draw", &objectViewSelection,
                              objectSelected, IM_ARRAYSIZE(objectSelected));

                ImGui ::Combo("Choose Wind Waker Asset", &windWakerSelection,
                              windWakerSelected,
                              IM_ARRAYSIZE(windWakerSelected));
            }

            if (ImGui::CollapsingHeader("Lights")) {
                ImGui::SliderFloat("FlashLight Radius", &flashLightRadius, 1.0,
                                   20.0f);
                ImGui::SliderFloat("Attenuation Linear ", &attenuation[1],
                                   0.014, 0.7);
                ImGui::SliderFloat("Attenuation Quadratic ", &attenuation[2],
                                   0.000007, 1.8);
                ImGui::ColorEdit3("Ambient Light Color", ambientLightColor);
                ImGui::ColorEdit3("Diffuse Light Color", diffuseLightColor);
                ImGui::ColorEdit3("Specular Light Color", specularLightColor);
            }

            if (ImGui::CollapsingHeader("Camera Settings")) {
                ImGui::SliderFloat("Camera Speed", &cam.MovementSpeed,
                                   Camera::MIN_SPEED, Camera::MAX_SPEED);
                if (ImGui ::Combo("Camera Mode", &cameraModeSelection,
                                  cameraModeSelected,
                                  IM_ARRAYSIZE(cameraModeSelected))) {
                    cam.type = (CameraType)cameraModeSelection;
                }
                if (ImGui ::Combo("Projection", &projectionModeSelection,
                                  projectionModeSelected,
                                  IM_ARRAYSIZE(projectionModeSelected))) {
                }
            }

            ImGui::End();

// -- UI Render
#pragma endregion
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // -- Buffer Swap & Events
            glfwSwapBuffers(window);
            glfwPollEvents();  // We will register event callbacks soon :)
        }
        glDeleteFramebuffers(1, &framebuffer);

        ExitEngine();
        return 0;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        LOG("Switching Engine Mode");
        if (appMode == ApplicationMode::Engine)
            appMode = ApplicationMode::Game;
        else if (appMode == ApplicationMode::Game)
            appMode = ApplicationMode::Engine;
        return;
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            game.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            game.Keys[key] = false;
    }
}

void GetInputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (cursorVisible) {
            cursorVisible = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            return;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        if (!cursorVisible) {
            cursorVisible = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
            return;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        userUpDown += 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        userUpDown -= 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        userLeftRight -= 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        userLeftRight += 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboardInputs(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboardInputs(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboardInputs(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboardInputs(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.ProcessKeyboardInputs(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.ProcessKeyboardInputs(DOWN, deltaTime);
}

TerrainData GetTerrainDataFromHeightMap(const char* filePath) {
    TerrainData terrainData;

    int width, height, channels;
    unsigned short* data16 =
        stbi_load_16(filePath, &width, &height, &channels, 0);

    if (data16) {
        LOG(" [Terrain] - LoadHeightmap at", filePath, " -- W: ", width,
            " --H: ", height, " --Channels: ", channels);
    } else {
        LOG_ERROR(LogCategory::Engine,
                  "Failed to load heightmap data:", filePath);
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
    shader.SetVec3("dirLight.ambient",
                   glm::vec3(ambientLightColor[0], ambientLightColor[1],
                             ambientLightColor[2]));
    shader.SetVec3("dirLight.diffuse",
                   glm::vec3(diffuseLightColor[0], diffuseLightColor[1],
                             diffuseLightColor[2]));
    shader.SetVec3("dirLight.specular",
                   glm::vec3(specularLightColor[0], specularLightColor[1],
                             specularLightColor[2]));

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
    shader.SetFloat("flashLight.cutOff",
                    glm::cos(glm::radians(flashLightRadius)));
    shader.SetFloat("flashLight.outerCutOff",
                    glm::cos(glm::radians(flashLightRadius + 2.5f)));
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
        unsigned char* data =
            stbi_load(facePaths[i].c_str(), &w, &h, &channels, 0);

        if (data) {
            LOG_INFO(LogCategory::Texture, " CUBEMAP Loaded : ", facePaths[i]);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            LOG_ERROR(LogCategory::Texture, "Cubemap failed to load at path",
                      facePaths[i]);
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