#ifndef SUB_TEXTURE_HPP
#define SUB_TEXTURE_HPP

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace mtrs::res
{

struct SubTexture
{
    glm::vec2 lb_vertex = glm::vec2(0.0f);
    glm::vec2 rt_vertex = glm::vec2(1.0f);
};

}

#endif
