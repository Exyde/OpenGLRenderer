#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image.h"

void LogMaxVertexAttributes(){
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Max Vertex Attributes: " << maxAttributes << std::endl;
}


float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 

float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};


void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
    glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);

    Shader baseShader("../Shaders/vert.vs", "../Shaders/frag.fs");


    //-- Textures chapter
    unsigned int woodTexture;
    glGenTextures(1, &woodTexture);
    glBindTexture(GL_TEXTURE_2D, woodTexture);

    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texWidth, texHeight, nrChannels;
    unsigned char *data = stbi_load("../Resources/Textures/container.jpg", &texWidth, &texHeight, &nrChannels, 0);

    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    } else{
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // -- Element Buffer Object (EBO) for indices
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // -- VAO instead.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // -- 1 : Bind VAO
    glBindVertexArray(VAO);

    // -- 2 : Copy data to buffer
    unsigned int VBO;
    glGenBuffers (1, &VBO); // Create the buffer behind the scenes and return an ID handle
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // just binding to GL ARRAY BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy data to buffer memory - We choose memory opti with STATIC

    
    // -- Vertex Attribute Setup
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float))); // -- Layout, Size of data, type, normalized, stride, offset pointer
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // -- Layout, Size of data, type, normalized, stride, offset pointer
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // -- Layout, Size of data, type, normalized, stride, offset pointer
    glEnableVertexAttribArray(2);

    // -- Render Loop
    while (!glfwWindowShouldClose(window)){
        // -- Input Handling
        GetInputs(window);

        // -- Actual Rendering
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT); // -- You can clean color, depth and stencil buffer !

        baseShader.Use();

        //glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // -- Buffer Swap & Events
        glfwSwapBuffers(window);
        glfwPollEvents(); // We will register event callbacks soon :)
    }


    // -- Exit
    glfwTerminate();
    return 0;

}

