#include "ShaderReloader.h"

#include <iostream>

ShaderReloader::ShaderReloader(Shader& _shader) {
    this->shader = &_shader;

    try {
        vLastWrite = fs::last_write_time(shader->vertexSaved);
        fLastWrite = fs::last_write_time(shader->fragmentSaved);
    } catch (const std::exception& e) {
        std::cerr << "Error initializing ShaderReloader: " << e.what()
                  << std::endl;
    }
}

void ShaderReloader::Reload() { shader->Reload(); }

bool ShaderReloader::CheckForChanges() {
    try {
        auto newVWrite = fs::last_write_time(shader->vertexSaved);
        auto newFWrite = fs::last_write_time(shader->fragmentSaved);

        if (newVWrite != vLastWrite || newFWrite != fLastWrite) {
            vLastWrite = newVWrite;
            fLastWrite = newFWrite;
            Reload();
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking shader file changes: " << e.what()
                  << std::endl;
    }

    return false;
}
