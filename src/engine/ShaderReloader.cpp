#include "ShaderReloader.h"
#include <iostream>

ShaderReloader::ShaderReloader(const std::string& vertexPath, const std::string& fragmentPath)
    : vPath(vertexPath), fPath(fragmentPath)
{
    try {
        vLastWrite = fs::last_write_time(vPath);
        fLastWrite = fs::last_write_time(fPath);
    }
    catch (const std::exception& e) {
        std::cerr << "Error initializing ShaderReloader: " << e.what() << std::endl;
    }
}

bool ShaderReloader::CheckForChanges()
{
    try {
        auto newVWrite = fs::last_write_time(vPath);
        auto newFWrite = fs::last_write_time(fPath);

        if (newVWrite != vLastWrite || newFWrite != fLastWrite) {
            vLastWrite = newVWrite;
            fLastWrite = newFWrite;
            return true; // un fichier a changé
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error checking shader file changes: " << e.what() << std::endl;
    }

    return false;
}
