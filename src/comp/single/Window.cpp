#include "comp/single/Window.hpp"

#include <utility>

namespace mtrs::comp
{

Window::Window(Window &&other) noexcept
{
    poiter = other.poiter;
    other.poiter = nullptr;
    size = std::move(other.size);
    name = other.name;
    other.name = nullptr;
}

Window &Window::operator=(Window &&other) noexcept
{
    if(this != &other)
    {
        poiter = other.poiter;
        other.poiter = nullptr;
        size = std::move(other.size);
        name = other.name;
        other.name = nullptr;
    }
    return *this;
}

Window::Window(const glm::uvec2 &size, const char *name)
: size(size), name(name)
{
}

}