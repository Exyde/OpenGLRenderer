cmake_minimum_required(VERSION 3.16)
project(OpenGLRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Répertoire de sortie du binaire
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})


# === Include paths ===
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glad
    ${CMAKE_SOURCE_DIR}/include/GLFW
    ${CMAKE_SOURCE_DIR}/include/glm
)

# === Source files ===
file(GLOB_RECURSE ENGINE_SRC ${CMAKE_SOURCE_DIR}/src/engine/*.cpp)
file(GLOB_RECURSE IMGUI_SRC ${CMAKE_SOURCE_DIR}/src/imgui/*.cpp)

add_executable(OpenGLRenderer
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/glad.c
    ${ENGINE_SRC}
    ${IMGUI_SRC}
)

# === Libraries ===
target_link_directories(OpenGLRenderer PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(OpenGLRenderer PRIVATE ${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a opengl32)

# Assimp
target_include_directories(OpenGLRenderer PRIVATE "C:/msys64/mingw64/include")
target_link_directories(OpenGLRenderer PRIVATE "C:/msys64/mingw64/lib")
target_link_libraries(OpenGLRenderer PRIVATE libassimp.dll.a)


# === Copy runtime DLLs after build ===
add_custom_command(TARGET OpenGLRenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/glfw3.dll
        $<TARGET_FILE_DIR:OpenGLRenderer>
)

# === Nice output ===
message(STATUS "✅ OpenGLRenderer ready to build")
