#pragma once

#include "glm.hpp"

enum class Direction { UP, RIGHT, DOWN, LEFT };

const glm::vec2 compass[] = {
    glm::vec2(0.0, 1.0),   // up
    glm::vec2(1.0, 0.0),   // right
    glm::vec2(0.0, -1.0),  // down
    glm::vec2(-1.0, 0.0),  // left
};

static inline Direction GetVectorClosestDirection(glm::vec2 vector) {
    float max = 0.0f;
    unsigned int bestMatch = -1;

    for (unsigned int i = 0; i < 4; i++) {
        float dot = glm::dot(glm::normalize(vector), compass[i]);
        if (dot > max) {
            max = dot;
            bestMatch = i;
        }
    }

    return (Direction)bestMatch;
}
