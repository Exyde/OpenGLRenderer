#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\0";
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

    // -- Vertex Shader 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int succes;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succes);
    if (!succes) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // -- Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succes);
    if (!succes) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // -- Shader object aka final shader 
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succes);
    if(!succes) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // -- Layout, Size of data, type, normalized, stride, offset pointer
    glEnableVertexAttribArray(0);

    // -- Set shader for next draw calls
    glUseProgram(shaderProgram);

    // -- Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // -- Render Loop
    while (!glfwWindowShouldClose(window)){
        // -- Input Handling
        GetInputs(window);

        // -- Actual Rendering
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT); // -- You can clean color, depth and stencil buffer !

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // -- Buffer Swap & Events
        glfwSwapBuffers(window);
        glfwPollEvents(); // We will register event callbacks soon :)
    }


    // -- Exit
    glfwTerminate();
    return 0;

}

