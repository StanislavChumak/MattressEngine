#include "comp/single/Window.hpp"

#include <utility>

namespace mtrs::comp
{

Window::Window(const glm::uvec2 &size, const char *name)
: size(size), name(name)
{
}

}