#include "GameLevel.h"

void GameLevel::Load(const char* filePath, unsigned int levelWidth,
                     unsigned int levelHeight) {
    LOG_INFO(logCat, "Game Level: Loading...");
    this->Bricks.clear();

    unsigned int tileCode;
    std::string line;
    std::ifstream fstream(filePath);
    TileData data;

    if (fstream) {
        while (std::getline(fstream, line))  // -- Read line in string buffer
        {
            std::istringstream sstream(line);
            TileRow row;

            // -- Read each word space separated
            while (sstream >> tileCode) {
                row.data.push_back(tileCode);
            }

            data.Rows.push_back(row);
        }

        if (data.Rows.size() > 0) {
            this->init(data, levelWidth, levelHeight);
        } else {
            LOG_ERROR(logCat,
                      "Failed to load correctly level from file:", filePath);
        }
    } else {
        LOG_ERROR(logCat, "Failed to open fstream @path: ", filePath);
    }
}

void GameLevel::Draw(SpriteRenderer& renderer) {
    for (auto obj : Bricks) {
        if (!obj.IsDestroyed) obj.Draw(renderer);
    }
}

bool GameLevel::IsCompleted() {
    for (auto obj : Bricks) {
        if (!obj.IsSolid && !obj.IsDestroyed) return false;
    }
    return true;
}

bool GameLevel::init(TileData data, unsigned int levelWidth,
                     unsigned int levelHeight) {
    if (levelWidth <= 0 || levelHeight <= 0) {
        LOG_ERROR(logCat, "Invalid level dimensions : ", levelWidth, ",",
                  levelHeight, ". Provide a positive value !");
        return false;
    }

    // -- Compute Bounds
    unsigned int height = data.GetHeight();
    unsigned int width = data.GetWidth();
    float cellWidth = levelWidth / static_cast<float>(width);
    float cellHeight = levelHeight / static_cast<float>(height);

    glm::vec2 cellSize(cellWidth, cellHeight);
    glm::vec4 solidColor(1.0, 0.0, 0.0, 1.0);
    glm::vec4 gameplayColor(0.25, 0.4, 0.6, 1.0);

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            int currentTileCode = data.GetTileCode(y, x);
            glm::vec2 pos(cellWidth * x, cellWidth * y);
            // -- Solid
            if (currentTileCode == 1) {
                GameObject brick(pos, cellSize,
                                 ResourceLoader::GetTexture2D("block_solid"),
                                 solidColor);
                brick.IsSolid = true;
                this->Bricks.push_back(brick);

                // -- Gameplay Tiles
            } else if (currentTileCode >= 1) {
                GameObject brick(pos, cellSize,
                                 ResourceLoader::GetTexture2D("block"),
                                 gameplayColor);
                this->Bricks.push_back(brick);
            }
        }
    }

    return true;
}
