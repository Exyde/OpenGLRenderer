#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
class ShaderReloader
{
public:
    ShaderReloader(const std::string& vertexPath, const std::string& fragmentPath);
    bool CheckForChanges();

private:
    std::string vPath;
    std::string fPath;
    fs::file_time_type vLastWrite;
    fs::file_time_type fLastWrite;
};
