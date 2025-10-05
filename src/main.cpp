#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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

    // -- Render Loop
    while (!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents(); // We will register event callbacks soon :)
    }


    // -- Exit
    glfwTerminate();
    return 0;

}

