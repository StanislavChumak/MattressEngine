#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "comp/Component.hpp"

namespace mtrs::comp
{
struct Transform
{
    glm::vec2 position;
    glm::vec2 scale_size;
    float rotation;

    glm::mat4 local_matrix{1.0f};
    glm::mat4 global_matrix{1.0f};

    bool dirty{true};

    Transform() = delete;
    Transform(COMPONENT_ARGS);

    void update_local_matrix();

    void set_position(glm::vec2 newPosition);
    void set_scale_size(glm::vec2 newScaleSize);
    void set_rotation(float newRotation);
};

}

#endif
