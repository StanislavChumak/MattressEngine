#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "glm/vec2.hpp"

#include "util/reactive/ReactiveValue.hpp"
#include "util/reactive/ReactiveLeaf.hpp"

namespace mtrs::comp
{

struct Cursor
{
    util::ReactiveValue<glm::uvec2> window_position;
    util::ReactiveValue<glm::uvec2, glm::uvec2, glm::uvec2, glm::uvec2, glm::uvec2> position;

    Cursor() = delete;
    ~Cursor() = default;
    Cursor(const Cursor &) = delete;
    Cursor &operator=(const Cursor&) = delete;
    Cursor(Cursor &&other) = delete;
    Cursor &operator=(Cursor &&other) = delete;

    Cursor(util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> &window_size,
        util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> &size_in_pixels,
        util::ReactiveValue<glm::uvec2, glm::uvec2, glm::uvec2> &offset_viewport);
};

}

#endif
