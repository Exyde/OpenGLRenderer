#pragma once
#include <vector>

struct TerrainData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int NUM_STRIPS;
    unsigned int NUM_VERTS_PER_STRIP;
};

TerrainData GetTerrainDataFromHeightMap(const char* filePath);

// -- Terrain
/*
TerrainData terrainData = GetTerrainDataFromHeightMap("Resources/Textures/iceland_heightmap.png");
glGenVertexArrays(1, &terrainVAO);
glBindVertexArray(terrainVAO);
glGenBuffers(1, &terrainVBO);
glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
glBufferData(GL_ARRAY_BUFFER, terrainData.vertices.size() * sizeof(float), &terrainData.vertices[0],
             GL_STATIC_DRAW);

// position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
glEnableVertexAttribArray(0);

glGenBuffers(1, &terrainEBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainData.indices.size() * sizeof(unsigned int), &terrainData.indices[0],
             GL_STATIC_DRAW);
             */

/*
//-- Draw Terrain
glBindVertexArray(terrainVAO);
bool drawTerrain = false;
if (drawTerrain) {
for (unsigned int strip = 0; strip < terrainData.NUM_STRIPS; ++strip) {
glDrawElements(GL_TRIANGLE_STRIP, terrainData.NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT,
          (void*)(sizeof(unsigned int) * terrainData.NUM_VERTS_PER_STRIP * strip));
}
}
*/
/*
TerrainData GetTerrainDataFromHeightMap(const char* filePath) {
    TerrainData terrainData;

    int width, height, channels;
    unsigned short* data16 = stbi_load_16(filePath, &width, &height, &channels, 0);

    if (data16) {
        LOG(" [Terrain] - LoadHeightmap at", filePath, " -- W: ", width, " --H: ", height, " --Channels: ", channels);
    } else {
        LOG_ERROR(LogCategory::Engine, "Failed to load heightmap data:", filePath);
    }

    std::vector<float> vertices;
    float yScale = 50.0f;
    float yShift = 0.0F;

    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            // -- Data 16
            unsigned short value16 = data16[width * i + j];
            float h = (float)value16 / 65535.0F;

            float worldX = (i - width / 2.0F);
            float worldZ = (j - height / 2.0F);
            float worldY = h * yScale - yShift;

            vertices.push_back(worldX);
            vertices.push_back(worldY);
            vertices.push_back(worldZ);
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < height - 1; i++) {
        for (unsigned int j = 0; j < width; j++) {
            for (unsigned int k = 0; k < 2; k++)  // for each side of the strip
            {
                indices.push_back(j + width * (i + k));
            }
        }
    }

    terrainData.vertices = vertices;
    terrainData.indices = indices;
    terrainData.NUM_STRIPS = height - 1;
    terrainData.NUM_VERTS_PER_STRIP = width * 2;

    stbi_image_free(data16);

    return terrainData;
}
    */