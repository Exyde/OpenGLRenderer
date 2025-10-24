#pragma once
#include <filesystem>
#include <string>

#include "Shader.h"

namespace fs = std::filesystem;
class ShaderReloader {
   public:
    ShaderReloader(Shader& shader);
    bool CheckForChanges();

   private:
    Shader* shader;
    void Reload();
    fs::file_time_type vLastWrite;
    fs::file_time_type fLastWrite;
};
