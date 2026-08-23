#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "comp/Component.hpp"

#include "util/type/react/ReactiveStruct.hpp"
#include "util/type/react/ReactiveSet.hpp"

namespace mtrs::comp
{

struct Transform : public Component<Transform>
{
    react::ReactiveStruct<glm::vec2, 2> position;
    react::ReactiveStruct<glm::vec2, 2> scale_size;
    react::ReactiveValue<float, 2> rotation;

    react::ReactiveSet<glm::mat4, 4, glm::vec2, glm::vec2, float, glm::mat4> matrix;

    Transform() = delete;
    Transform(COMPONENT_ARGS);

    static constexpr const char *get_type_name_imp() noexcept { return "Transform"; }
};

}

#endif
