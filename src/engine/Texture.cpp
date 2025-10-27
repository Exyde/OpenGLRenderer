#include "Texture.h"

#include "Logger.h"
Texture::Texture(const char* filePath, int clampMode, bool useAlpha,
                 std::string type) {
    glGenTextures(1, &ID);
    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);

    this->type = type;

    if (data) {
        if (useAlpha) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, data);

        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        Logger::Log(LogCategory::Texture, LogLevel::Info,
                    "Loaded : ", filePath);

    } else {
        LOG_ERROR(LogCategory::Texture, "Failed to load texture : ", filePath);
    }
    stbi_image_free(data);
}

void Texture::Bind() { glBindTexture(GL_TEXTURE_2D, ID); }

Texture::~Texture() {}