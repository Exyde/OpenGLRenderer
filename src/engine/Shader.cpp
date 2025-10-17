#include "Shader.h"

std::string Shader::LoadShaderCodeFromFile(const char* filePath) {
    // std::cout << "Loading shader code : " << filePath << std::endl;
    std::string shaderCode = "";
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }

    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
                  << std::endl;
    }

    return shaderCode;
}

int Shader::CompileShaderFromCode(std::string vertexCode,
                                  std::string fragmentCode) {
    // std::cout << "Compiling Shader Program..." << std::endl;

    // -- Compile Shader File
    int vertex, fragment;
    int success;
    char infoLog[512];

    const char* vertexBuffer = vertexCode.c_str();
    const char* fragmentBuffer = fragmentCode.c_str();

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexBuffer, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentBuffer, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // -3. Program
    int id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    // Check for linking errors
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    std::cout << "[SHADER] - Shader created succesfully at ID : " << id << " ! "
              << std::endl;
    return id;
}

int Shader::CreateShaderFromFiles(const char* vertexPath,
                                  const char* fragmentPath) {
    // std::cout << "Create Shader Program..." << std::endl;
    this->vertexSaved = vertexPath;
    this->fragmentSaved = fragmentPath;
    // -- Load Shaders File
    std::string vShaderCode = LoadShaderCodeFromFile(vertexPath);
    std::string fShaderCode = LoadShaderCodeFromFile(fragmentPath);

    // -- Compile & Create Shader Program
    int id = CompileShaderFromCode(vShaderCode, fShaderCode);
    return id;
}

void Shader::Reload() {
    std::cout << "Reloading Shader : " << vertexSaved << " - " << fragmentSaved
              << std::endl;

    int reloadedID = CreateShaderFromFiles(vertexSaved, fragmentSaved);

    if (reloadedID) {
        glDeleteProgram(ID);
        ID = reloadedID;
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::cout << "Shader Construction : " << vertexPath << " - " << fragmentPath
              << std::endl;
    ID = CreateShaderFromFiles(vertexPath, fragmentPath);
}

void Shader::Use() { glUseProgram(ID); }
void Shader::SetBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

Shader::~Shader() {}