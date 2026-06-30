#include "comp/core/Transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include <fstream>

#include "comp_struct/Transform.struct"

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
    Trancform_sc transform;
    file.read(reinterpret_cast<char*>(&transform), sizeof(transform));
    position.set({transform.pos_x, transform.pos_y});
    scale_size.set({transform.scale_size_x, transform.scale_size_y});
    rotation.set(transform.rotation);
    matrix.update();
}

}