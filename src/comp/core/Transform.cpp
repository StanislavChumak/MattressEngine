#include "comp/core/Transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "util/type/prs/comp/Transform.hpp"

#include <cstring>

namespace mtrs::comp
{

Transform::Transform(COMPONENT_ARGS)
: matrix([](const glm::vec2 *pos, const glm::vec2 *sc_size, const float *rot, const glm::mat4*)
    {
        glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(*pos, 0.0f));
        m = glm::rotate(m, glm::radians(*rot), glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::scale(m, glm::vec3(*sc_size, 1.0f));
        return m;
    }, &position, &scale_size, &rotation, nullptr)
{
    prs::Transform transform;
    std::memcpy(&transform, file_data, sizeof(transform));
    position.set({transform.pos_x, transform.pos_y});
    scale_size.set({transform.scale_size_x, transform.scale_size_y});
    rotation.set(transform.rotation);

    position.add_observer(&matrix);
    scale_size.add_observer(&matrix);
    rotation.add_observer(&matrix);

    matrix.update();
}

uint32_t Transform::get_prs_size_imp() noexcept
{
    return sizeof(prs::Transform);
}

}