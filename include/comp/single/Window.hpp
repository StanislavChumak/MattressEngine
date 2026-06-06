#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/vec2.hpp"

namespace mtrs::comp
{

struct Window {
    GLFWwindow *poiter = nullptr;
    glm::uvec2 size;
    const char *name;
    unsigned int scale;

    Window() = delete;
    Window(const glm::uvec2 &size, const char *name, unsigned int scale)
    :size(size), name(name), scale(scale) {}
};

}

#endif
