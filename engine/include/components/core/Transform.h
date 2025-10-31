#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/vec2.hpp"

struct Transform {
    glm::ivec2 position = glm::ivec2(0);
    glm::ivec2 size = glm::ivec2(0);
    float rotation = 0.f;
};

#endif
