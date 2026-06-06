#include "comp/core/Transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include <fstream>

#include "mtrsstruct/comp_struct/Transform.struct"

namespace mtrs::comp
{

Transform::Transform(COMPONENT_ARGS)
{
    Trancform_sc transform;
    file.read(reinterpret_cast<char*>(&transform), sizeof(transform));
    position.x = transform.pos_x; 
    position.y = transform.pos_y;
    scale_size.x = transform.scale_size_x;
    scale_size.y = transform.scale_size_y;
    rotation = transform.rotation;
}

void Transform::update_local_matrix()
{
    local_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    local_matrix = glm::rotate(local_matrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    local_matrix = glm::scale(local_matrix, glm::vec3(scale_size, 1.0f));
    dirty = false;
}

void Transform::set_position(glm::vec2 newPosition) { dirty = true; position = newPosition; }
void Transform::set_scale_size(glm::vec2 newScaleSize) { dirty = true; scale_size = newScaleSize; }
void Transform::set_rotation(float newRotation) {dirty = true; rotation = newRotation; }

}