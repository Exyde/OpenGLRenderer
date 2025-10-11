#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include "stb_image.h"
#include <cmath>


#include <iostream>


glm::vec3 Vec3 (float x, float y, float z) { return glm::vec3(x, y, z);}
glm::vec3 Vec3(float xyz) { return Vec3(xyz, xyz, xyz);}


float deltaTime = 0.0f;
float lastFrame = 0.0f;
float CAM_SPEED = 5.0F;
float FOV = 45.0F;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraTarget  = Vec3(0);
glm::vec3 DirectionToCamera = glm::normalize(cameraPos - cameraTarget); // - Change name maybe 
glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize (glm::cross(WorldUp, DirectionToCamera));
//glm::vec3 cameraUp = glm::cross (DirectionToCamera, cameraRight);
glm::vec3 cameraFront = Vec3(0.0f, 0.0f, -1.0F);
glm::vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0F);

bool FIRST_MOUSE = true;
float MOUSE_SENSITIVITY =  0.1F;
float lastMouseX = 400;
float lastMouseY = 300;
float Pitch, Yaw, Roll;

float ElapsedTime(){return (float) glfwGetTime();}
float lerp(float a, float b, float f) 
{
    return (a * (1.0 - f)) + (b * f);
}

void LogMaxVertexAttributes(){
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Max Vertex Attributes: " << maxAttributes << std::endl;
}

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


/// ---- CALLBACK OPEN GL ---- ////////
void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset){
    FOV -= (float)yoffset;
    if (FOV < 1.0F) FOV = 1.0F;
    if (FOV > 45.0F) FOV = 45.0f;
}

void Mouse_Callback(GLFWwindow* window, double xpos, double ypos){

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

    deltaX *= MOUSE_SENSITIVITY;
    deltaY *= MOUSE_SENSITIVITY;

    Yaw += deltaX;
    Pitch += deltaY;

    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    glm::vec3 newCameraDirection;
    newCameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    newCameraDirection.y = sin(glm::radians(Pitch));
    newCameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    // -- Update Camera Front
    cameraFront = glm::normalize(newCameraDirection);
}




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

void GetInputs(GLFWwindow* window){

    // -- Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ // Else is GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        userUpDown += 0.0001;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        userUpDown -= 0.0001;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        userLeftRight += 0.0001;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        userLeftRight -= 0.0001;
    }

    const float cameraSpeed = CAM_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

        // -- Cross for direction between up and forward, normalize to make it a unity vector <3
        // -- You're getting good with dot & cross I believe ! [Remember the maxim: to understand shit, im-ple-ment]
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main()
{
    if (!InitializeGLFW())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    uint32_t width = 800, height = 600;
    GLFWwindow* window = CreateWindow(width, height);

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
    glViewport(0, 0, width, height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);
    glfwSetScrollCallback(window, Scroll_Callback);
    stbi_set_flip_vertically_on_load(true);

    Shader baseShader("../Shaders/vert.vs", "../Shaders/frag.fs");

    // -- Enable Z Testing 
    glEnable(GL_DEPTH_TEST);

    // -- Cube Test
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

        glActiveTexture(GL_TEXTURE0);
        albedo.Bind();
        glActiveTexture(GL_TEXTURE1);
        mask.Bind();
        baseShader.Use();

        const float radius = 10.0f;
        float camX = sin(ElapsedTime()) * radius;
        float camZ = cos(ElapsedTime())* radius;
        glm::mat4 view;
        // -- Update Camera actually o/
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        if (userUpDown < 0) userUpDown = 0;
        if (userUpDown > 1) userUpDown = 1;
        if (userLeftRight < 0) userLeftRight = 0;
        if (userLeftRight > 1) userLeftRight = 1;


        baseShader.SetFloat("T", userUpDown);



        float cameraDistance = lerp(-5.0, -100.0f, userUpDown);
        float cameraX = lerp(-10.0f, 10.f, userLeftRight);
        float cameraY = lerp(-10.0f, 10.f, userLeftRight);
        // -- Model // View // Projections
        glm::mat4 modelMatrix = glm::mat4(1.0F);
        modelMatrix = glm::rotate(modelMatrix, ElapsedTime()* glm::radians(50.0f), glm::vec3(0.5f, 1.0, 0.0));
        glm::mat4 viewMatrix = glm::mat4(1.0F);
        viewMatrix = glm::translate(viewMatrix, glm::vec3(cameraX, cameraY, cameraDistance));

         // here FOV, aspect ratio, near and far plane for perspective (w scaled)
        glm::highp_mat4 perspectiveMatrix = glm::perspective(glm::radians(FOV), (float)width/(float)height, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(baseShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        unsigned int viewLoc = glGetUniformLocation(baseShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        unsigned int projLoc = glGetUniformLocation(baseShader.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));


        glBindVertexArray(CubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle + ElapsedTime() * 100 ), glm::vec3(1.0f, 0.3f, 0.5f));
            baseShader.SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
     
        // -- Buffer Swap & Events
        glfwSwapBuffers(window);
        glfwPollEvents(); // We will register event callbacks soon :)
    }


    // -- Exit
    glfwTerminate();
    return 0;

}

