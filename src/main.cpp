#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/Shader.h"
#include "engine/Texture.h"
#include "engine/stb_image.h"
#include "engine/Camera.h"
#include <cmath>
#include <iostream>
#include "engine/SlyMath.H"

enum ViewMode { Normal, Wireframe };

ViewMode viewMode = ViewMode::Normal;

/// --- UI Stuffs --------
bool UI_rotateStuff = false;

/// --- Lights Stuff ----
Vector3 LightPosition (2.0f, 1.5F, 2.0F);

#pragma region UserShitToMoveLaterAwayFromHere
const uint32_t SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
bool cursorVisible = false;
float userUpDown = 1.0;
float userLeftRight = 1.0;
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
#pragma endregion

Camera cam(Vector3(0.0f, 0.0f, -3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool FIRST_MOUSE = true;
float lastMouseX = 400;
float lastMouseY = 300;

float ElapsedTime(){return (float) glfwGetTime();}

void LogMaxVertexAttributes(){
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Max Vertex Attributes: " << maxAttributes << std::endl;
}

#pragma region CallbackOpenGL
void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset){
    if (cursorVisible) return;
    cam.ProcessMouseScroll(yoffset);
}

void Mouse_Callback(GLFWwindow* window, double xposin, double yposin){

    if (cursorVisible) return;
    float xpos = static_cast<float>(xposin);
    float ypos = static_cast<float>(yposin);

    //-- avoid first frame snapping (should have a bool for first frame ?)
    if (FIRST_MOUSE){
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
bool InitializeGLFW(){
    if (!glfwInit()){
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}

GLFWwindow* CreateWindow(uint32_t width, uint32_t height){
    GLFWwindow* window = glfwCreateWindow(width, height, "Open GL Renderer v0.0.1", NULL, NULL);
    return window;
}
#pragma endregion

void GetInputs(GLFWwindow* window){

    // -- Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ // Else is GLFW_RELEASE

        if (!cursorVisible){
            cursorVisible = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
            return;
        } else {
            cursorVisible = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        //glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        userUpDown += 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        userUpDown -= 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        userLeftRight -= 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
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
}

int main()
{
    if (!InitializeGLFW())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // -- Glad 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // -- Viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);
    glfwSetScrollCallback(window, Scroll_Callback);
    
    stbi_set_flip_vertically_on_load(true);
    Shader lightShader("../Shaders/light_source_vertex.vs", "../Shaders/light_source_frag.fs");
    Shader phongShader("../Shaders/vert.vs", "../Shaders/frag.fs");
    glEnable(GL_DEPTH_TEST);


    // -- Cube Datas
    unsigned int CubeVBO, CubeVAO;
    glGenVertexArrays(1, &CubeVAO);
    glGenBuffers(1, &CubeVBO);
    glBindVertexArray(CubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)   
    };

    // -- Light Data
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Texture albedo("../Resources/Textures/container.jpg", GL_CLAMP_TO_BORDER, false, "diffuse");
    Texture mask("../Resources/Textures/awesomeface.png", GL_CLAMP_TO_BORDER, true, "diffuse");
    Texture diffuse("../Resources/Textures/container_diffuse.png", GL_CLAMP_TO_BORDER, true, "diffuse");
    Texture specular("../Resources/Textures/container_specular.png", GL_CLAMP_TO_BORDER, true, "specular");
    Texture emissive("../Resources/Textures/container_emmisive.jpg", GL_CLAMP_TO_BORDER, false, "diffuse");
    Texture funky("../Resources/Textures/container_funky_specular.png", GL_CLAMP_TO_BORDER, false, "specular");
    phongShader.Use();
    phongShader.SetInt("albedo", 0);
    phongShader.SetInt("mask", 1);

    // -- IMGUI INIT
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // -- IMGUI EXPOSED
    float ambientLightColor[3] = {0.1, 0.1f, 0.1f};
    float diffuseLightColor[3] = {1.0, 1.0f, 1.0f};
    float specularLightColor[3] = {0.5, 0.5f, 0.5f};
    float attenuation [3] = {1.0, 0.09f, 0.032f};
    float lightOffsets[]{0.0, 0.0, 0.0};
    static const char* viewModes[]{"Normal", "Wireframe"};

    glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    glm::vec3 pointLightAmbient(0.05f);
    glm::vec3 pointLightDiffuse(0.8f);
    glm::vec3 pointLightSpecular(1.0f);

    // -- Render Loop
    while (!glfwWindowShouldClose(window)){
        // -- Input Handling
        GetInputs(window);

        // -- Time Update
        float currentFrame = ElapsedTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // -- Actual Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // -- You can clean color, depth and stencil buffer !

        // -- Transformed positions
        auto offsetedLightPos = LightPosition.GLM() + Vector3(lightOffsets[0], lightOffsets[1], lightOffsets[2]).GLM();

        // -- UI IMGUI
        // 1. New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // -- Textures Units ?
        glActiveTexture(GL_TEXTURE0);
        diffuse.Bind();
        glActiveTexture(GL_TEXTURE1);
        specular.Bind();
        glActiveTexture(GL_TEXTURE2);
        emissive.Bind();
        phongShader.Use();

        // -- Fun Lerp Things
        userUpDown = SlyMath::Clamp(userUpDown, 0.0f, 1.0f);
        userLeftRight = SlyMath::Clamp(userLeftRight, 0.0f, 1.0f);

        // -- Phong Shader 
        // -- Material
        phongShader.SetInt("mat.diffuse", 0);
        phongShader.SetInt("mat.specular", 1);
        phongShader.SetInt("mat.emissive", 2);
        phongShader.SetFloat("mat.shininess", 32.0F);

        // -- User Data
        phongShader.SetFloat("uTime", ElapsedTime() * userUpDown);
        phongShader.SetFloat("T", userLeftRight);
        phongShader.SetVec3("ViewPos", cam.Position.GLM());

        // -- Directionnal Light
        phongShader.SetVec3("dirLight.direction", offsetedLightPos);
        phongShader.SetVec3("dirLight.ambient", glm::vec3(ambientLightColor[0], ambientLightColor[1], ambientLightColor[2]));
        phongShader.SetVec3("dirLight.diffuse", glm::vec3(diffuseLightColor[0], diffuseLightColor[1], diffuseLightColor[2]));
        phongShader.SetVec3("dirLight.specular",glm::vec3(specularLightColor[0], specularLightColor[1], specularLightColor[2]));
 
        // -- Point Lights
        for (int i = 0; i < 4; i++){
            std::string lightTag = "";
            if (i == 0) lightTag = "pointLights[0]";
            if (i == 1) lightTag = "pointLights[1]";
            if (i == 2) lightTag = "pointLights[2]";
            if (i == 3) lightTag = "pointLights[3]";

            phongShader.SetVec3(lightTag.append(".position"), pointLightPositions[i]);
            phongShader.SetVec3(lightTag.append(".ambient"), pointLightAmbient);
            phongShader.SetVec3(lightTag.append(".diffuse"), pointLightDiffuse);
            phongShader.SetVec3(lightTag.append(".specular"),pointLightSpecular);
            phongShader.SetFloat(lightTag.append(".constant"), attenuation[0]);
            phongShader.SetFloat(lightTag.append(".linear"),attenuation[1]);
            phongShader.SetFloat(lightTag.append(".quadratic"), attenuation[2]);
        }
            
        // -- Todo : SpotLight 
        // -- Todo : Lamp Torch

        // -- Model // View // Projections -- GROUND
        glm::mat4 modelMatrix = glm::mat4(1.0F);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(100, 2, 100));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -1, 0));
        glm::mat4 viewMatrix = cam.GetViewMatrix();

        // here FOV, aspect ratio, near and far plane for perspective (w scaled)
        glm::highp_mat4 perspectiveMatrix = glm::perspective(glm::radians(cam.Fov), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // -- Sending Matrices to Shader
        unsigned int modelLoc = glGetUniformLocation(phongShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        unsigned int viewLoc = glGetUniformLocation(phongShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        unsigned int projLoc = glGetUniformLocation(phongShader.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
        glBindVertexArray(CubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // -- Central Cube -- 
        modelMatrix = glm::mat4(1.0F);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
        phongShader.SetMat4("model", modelMatrix);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
        glBindVertexArray(CubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // -- Multiples Floating Cubes
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (UI_rotateStuff){
                model = glm::rotate(model, glm::radians(angle + ElapsedTime() * 100 ), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            phongShader.SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // -- Light Object -- //
        lightShader.Use();
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, offsetedLightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.SetMat4("model", lightModel);
        lightShader.SetMat4("view", viewMatrix);
        lightShader.SetMat4("projection", perspectiveMatrix);
        lightShader.SetVec3("LightPos", offsetedLightPos);
        lightShader.SetVec3("LightColor", Vector3(diffuseLightColor[0], diffuseLightColor[1], diffuseLightColor[2]).GLM());
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        for (int i = 0; i < 4; i++){
            lightModel = glm::mat4(1.0F);
            lightModel = glm::translate(lightModel, pointLightPositions[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));
            lightShader.SetMat4("model", lightModel);
            lightShader.SetVec3("LightPos", pointLightPositions[i]);
            lightShader.SetVec3("LightColor", Vector3(diffuseLightColor[0], diffuseLightColor[1], diffuseLightColor[2]).GLM());

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // -- UI FOLLOW UP
        ImGui::Begin("Some User Interface for this tiny Renderer <3 - Hello Bluesky !");
        ImGui::Text("Hello World ! Have you seen that france has Lecornu Government 2.0 ?");
        ImGui::Checkbox("Rotate Stuffs", &UI_rotateStuff);
        static int viewMode = -1;
        if (ImGui::Combo("Choose View Mode", &viewMode, viewModes, IM_ARRAYSIZE(viewModes))){
            if (viewMode == 0){
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else if (viewMode == 1){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
        ImGui::SliderFloat("TimeScale", &userUpDown, 0.0, 1.0);
        ImGui::SliderFloat("User T", &userLeftRight, 0.0, 1.0);
        ImGui::SliderFloat("Attenuation Linear ", &attenuation[1], 0.014, 0.7);
        ImGui::SliderFloat("Attenuation Quadratic ", &attenuation[2], 0.000007, 1.8);
        ImGui::ColorEdit3("Ambient Light Color", ambientLightColor);
        ImGui::ColorEdit3("Diffuse Light Color", diffuseLightColor);
        ImGui::ColorEdit3("Specular Light Color", specularLightColor);
        ImGui::SliderFloat3("Light Offsets", (float*)lightOffsets, -10, 10);
        ImGui::End();
   
        // -- UI Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // -- Buffer Swap & Events
        glfwSwapBuffers(window);
        glfwPollEvents(); // We will register event callbacks soon :)
    }

    // -- Exit
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;

}
