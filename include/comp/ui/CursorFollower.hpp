#ifndef CURSOR_FOLLOWER_HPP
#define CURSOR_FOLLOWER_HPP

#include "glm/vec2.hpp"

#include "comp/Component.hpp"

namespace mtrs::comp
{

struct CursorFollower
{
    glm::dvec2 offset;

    CursorFollower() = delete;
    CursorFollower(COMPONENT_ARGS);
};

}

#endif
