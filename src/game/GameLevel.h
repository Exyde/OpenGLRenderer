#pragma once

#include <vector>

#include "../engine/ResourceLoader.h"
#include "GameObject.h"

struct TileRow {
    std::vector<unsigned int> data;
};

struct TileData {
    std::vector<TileRow> Rows;

    unsigned int GetHeight() { return Rows.size(); }
    unsigned int GetWidth() { return Rows[0].data.size(); }
    int GetTileCode(int y, int x) { return Rows[y].data[x]; }
};

class GameLevel {
   public:
    static inline LogCategory logCat = LogCategory::Game;
    std::vector<GameObject> Bricks;
    GameLevel() {}
    void Load(const char* filePath, unsigned int levelWidth,
              unsigned int levelHeight);
    void Draw(SpriteRenderer& renderer);
    bool IsCompleted();

   private:
    bool init(TileData data, unsigned int levelWidth, unsigned int levelHeight);
};