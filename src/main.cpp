#include "main.h"

int main() {
#pragma region Init

    // -- Global Initialisions
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
    glfwSetKeyCallback(window, Keyboard_Callback);
#pragma endregion

    while (!glfwWindowShouldClose(window)) {
        // -- Skip everything if windows not focused.
        if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            glfwPollEvents();
            continue;
        }

        if (appMode == ApplicationMode::Game) {
            game.Initialize();
            float deltaTime = 0.0f;
            float lastFrame = 0.0f;

            while (!glfwWindowShouldClose(window) && appMode == ApplicationMode::Game) {
                float currentFrame = ElapsedTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                glfwPollEvents();

                game.ProcessInput(deltaTime);
                game.Update(deltaTime);
                game.Render();

                glfwSwapBuffers(window);
            }

            game.Exit();
            ResourceLoader::Clear();
            glfwTerminate();
            return 0;
        }

        if (appMode == ApplicationMode::Engine) {
            engine.Initialize();

            while (!glfwWindowShouldClose(window) && appMode == ApplicationMode::Engine) {
                float currentFrame = ElapsedTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                glfwPollEvents();

                engine.ProcessInput(deltaTime);
                engine.Update(deltaTime);
                engine.Render(deltaTime);

                glfwSwapBuffers(window);
            }

            engine.Exit();
            return 0;
        }
    }
}

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

    GLFWwindow* window = glfwCreateWindow(width, height, "Open GL Renderer v0.0.1", NULL, NULL);
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

#pragma region CallbackOpenGL
void Keyboard_Callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // -- Close Application
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (cursorVisible) {
            cursorVisible = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            return;
        }
    }

    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
        if (!cursorVisible) {
            cursorVisible = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
            return;
        }
    }

    // -- Toggle Between Engine & Game Mode
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        LOG("Switching Engine Mode");
        if (appMode == ApplicationMode::Engine)
            appMode = ApplicationMode::Game;
        else if (appMode == ApplicationMode::Game)
            appMode = ApplicationMode::Engine;
        return;
    }

    // -- Register inputs -- // TODO : Check, might bug ?
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            game.Keys[key] = engine.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            game.Keys[key] = engine.Keys[key] = true;
    }
}

void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height) {
    CURRENT_WIDTH = width;
    CURRENT_HEIGHT = height;
    glViewport(0, 0, width, height);

    if (appMode == ApplicationMode::Game)
        return;

    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (appMode == ApplicationMode::Game)
        return;

    if (cursorVisible)
        return;
    // cam.ProcessMouseScroll(yoffset);
    cam.MovementSpeed += (float)yoffset;
}

void Mouse_Callback(GLFWwindow* window, double xposin, double yposin) {
    if (appMode == ApplicationMode::Game)
        return;

    if (cursorVisible)
        return;
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
