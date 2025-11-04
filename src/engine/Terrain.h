#pragma once

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