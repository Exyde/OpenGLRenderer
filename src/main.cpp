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


/// --- UI Stuffs --------
bool UI_rotateStuff = false;


#pragma region UserShitToMoveLaterAwayFromHere
const uint32_t SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
bool cursorVisible = false;
float userUpDown = 1.0;
float userLeftRight = 1.0;
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    Shader baseShader("../Shaders/vert.vs", "../Shaders/frag.fs");
    glEnable(GL_DEPTH_TEST);

    // -- Cube Datas
    unsigned int CubeVBO, CubeVAO;
    glGenVertexArrays(1, &CubeVAO);
    glGenBuffers(1, &CubeVBO);
    glBindVertexArray(CubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    Texture albedo("../Resources/Textures/container.jpg", GL_CLAMP_TO_BORDER, false, "diffuse");
    Texture mask("../Resources/Textures/awesomeface.png", GL_CLAMP_TO_BORDER, true, "diffuse");
    baseShader.Use();
    baseShader.SetInt("albedo", 0);
    baseShader.SetInt("mask", 1);

    // -- IMGUI INIT
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float mainTint[4] = {0.0, 1.0f, 0.3f, 1.0F};

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

        // -- UI IMGUI
        // 1. New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // -- Textures Units ?
        glActiveTexture(GL_TEXTURE0);
        albedo.Bind();
        glActiveTexture(GL_TEXTURE1);
        mask.Bind();
        baseShader.Use();

        // -- Fun Lerp Things
        userUpDown = SlyMath::Clamp(userUpDown, 0.0f, 1.0f);
        userLeftRight = SlyMath::Clamp(userLeftRight, 0.0f, 1.0f);
        baseShader.SetFloat("T", userUpDown);
        glUniform4f(glGetUniformLocation(baseShader.ID, "Tint"), mainTint[0], mainTint[1], mainTint[2], mainTint[3]);

        // -- Model // View // Projections
        glm::mat4 modelMatrix = glm::mat4(1.0F);
        modelMatrix = glm::rotate(modelMatrix, ElapsedTime()* glm::radians(50.0f), glm::vec3(0.5f, 1.0, 0.0));
        
        glm::mat4 viewMatrix = cam.GetViewMatrix();

        // here FOV, aspect ratio, near and far plane for perspective (w scaled)
        glm::highp_mat4 perspectiveMatrix = glm::perspective(glm::radians(cam.Fov), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // -- Sending Matrices to Shader
        unsigned int modelLoc = glGetUniformLocation(baseShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        unsigned int viewLoc = glGetUniformLocation(baseShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        unsigned int projLoc = glGetUniformLocation(baseShader.ID, "projection");
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
            baseShader.SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // -- UI FOLLOW UP
            ImGui::Begin("Some User Interface for this tiny Renderer <3 - Hello Bluesky !");
            ImGui::Text("Hello World ! Have you seen that france has Lecornu Government 2.0 ?");
            ImGui::Checkbox("Rotate Stuffs", &UI_rotateStuff);
            ImGui::SliderFloat("User Up Down", &userUpDown, 0.0, 1.0);
            ImGui::SliderFloat("User Left Right", &userLeftRight, 0.0, 1.0);
            ImGui::ColorEdit4("Main Tint", mainTint);
            ImGui::End();
   
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
