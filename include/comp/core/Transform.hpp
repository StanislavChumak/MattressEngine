#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "comp/Component.hpp"

#include "util/reactive/ReactiveValue.hpp"

namespace mtrs::comp
{

struct Transform
{
    util::ReactiveValue<glm::vec2> position;
    util::ReactiveValue<glm::vec2> scale_size;
    util::ReactiveValue<float> rotation;

    util::ReactiveValue<glm::mat4, glm::vec2, glm::vec2, float, glm::mat4> matrix;

    Transform() = delete;
    Transform(COMPONENT_ARGS);
};

}

#endif
