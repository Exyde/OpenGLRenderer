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
    std::string vPath;
    std::string fPath;
    fs::file_time_type vLastWrite;
    fs::file_time_type fLastWrite;
};
