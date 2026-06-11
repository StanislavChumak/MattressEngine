#include "comp/single/Cursor.hpp"

#include <utility>

namespace mtrs::comp
{

Cursor::Cursor(Cursor &&other) noexcept
: position(std::move(other.position))
{
    window_position = std::move(other.window_position);
}

Cursor &Cursor::operator=(Cursor &&other) noexcept
{
    if(this != &other)
    {
        window_position = std::move(other.window_position);
        position = std::move(other.position);
    }
    return *this;
}

Cursor::Cursor(util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> &window_size,
    util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> &size_in_pixels,
    util::ReactiveValue<glm::uvec2, glm::uvec2, glm::uvec2> &offset_viewport)
: position([](const glm::uvec2 *win_pos, const glm::uvec2 *win_sz,
        const glm::uvec2 *sz_pxs, const glm::uvec2 *offset)
    {
        double ratio;
        if(offset->x == 0) ratio = static_cast<double>(win_sz->x) / sz_pxs->x;
        else ratio = static_cast<double>(win_sz->y) / sz_pxs->y;
        return static_cast<glm::uvec2>(static_cast<glm::dvec2>(*win_pos - *offset) / ratio);
    }, &window_position, &window_size, &size_in_pixels, &offset_viewport)
{
    window_size.add_observer(&position);
    size_in_pixels.add_observer(&position);
}

}