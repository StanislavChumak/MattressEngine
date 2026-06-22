#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/vec2.hpp"

#include "util/reactive/ReactiveLeaf.hpp"

namespace mtrs::comp
{

struct Window
{
    GLFWwindow *poiter = nullptr;
    const char *name;
    util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> size;

    Window() = delete;
    ~Window() = default;
    Window(const Window &) = delete;
    Window &operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window &operator=(Window&&) = delete;

    Window(const glm::uvec2 &size, const char *name);
};

}

#endif
