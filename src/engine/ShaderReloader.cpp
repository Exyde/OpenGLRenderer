#include "ShaderReloader.h"

#include <iostream>

ShaderReloader::ShaderReloader(Shader& _shader) {
    this->shader = &_shader;

    try {
        // LOG_INFO(LogCategory::Shader, "Created Reloader for :", shader->vertexSaved);
        vLastWrite = fs::last_write_time(shader->vertexSaved);
        fLastWrite = fs::last_write_time(shader->fragmentSaved);
    } catch (const std::exception& e) {
        LOG_ERROR(LogCategory::Shader, "Failed initializing ShaderReloader: ", e.what());
    }
}

void ShaderReloader::Reload() {
    shader->Reload();
}

void ShaderReloader::CheckForChanges() {
    try {
        auto newVWrite = fs::last_write_time(shader->vertexSaved);
        auto newFWrite = fs::last_write_time(shader->fragmentSaved);

        if (newVWrite != vLastWrite || newFWrite != fLastWrite) {
            vLastWrite = newVWrite;
            fLastWrite = newFWrite;
            Reload();
        }
    } catch (const std::exception& e) {
        LOG_ERROR(LogCategory::Shader, "Failed checking file changes : ", e.what());
    }
}
