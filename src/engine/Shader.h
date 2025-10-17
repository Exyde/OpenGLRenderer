#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
   public:
    int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Reload();
    void Use();
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat2(const std::string& name, const glm::mat2& value) const;
    void SetMat3(const std::string& name, const glm::mat3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;

    // -- Todo : Getters
    const char* vertexSaved;
    const char* fragmentSaved;

   private:
    std::string LoadShaderCodeFromFile(const char* filePath);
    int CompileShaderFromCode(std::string vertexCode, std::string fragmentCode);
    int CreateShaderFromFiles(const char* vertexPath, const char* fragmentPath);
};

#endif  // SHADER_H