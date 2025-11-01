#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "rapidjson/document.h"
#include "glm/vec2.hpp"

class ResourceManager;

struct Transform {
    glm::ivec2 position;
    glm::ivec2 size;
    float rotation;

    void fromJson(const rapidjson::Value& j, const ResourceManager &resource)
    {
        position.x = j["pos_x"].GetInt();
        position.y = j["pos_y"].GetInt();
        size.x = j["size_x"].GetInt();
        size.y = j["size_y"].GetInt();
        rotation = j["rotation"].GetFloat();
    }
};

#endif
