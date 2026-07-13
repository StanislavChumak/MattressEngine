#ifndef WINDOW_HPP
#define WINDOW_HPP

#ifndef SCRIPT
    #include "glad/glad.h"
    #include "GLFW/glfw3.h"
#else
    using GLFWwindow = void;
    using GLFWmonitor = void;
    using GLFWvidmode = void;
#endif

#include "glm/vec2.hpp"

#include "comp/Component.hpp"

#include "util/reactive/ReactiveStruct.hpp"

#include <vector>
#include <string>

namespace mtrs::comp
{

struct Window : public Component<Window>
{
    GLFWwindow *poiter = nullptr;
    GLFWmonitor* monitor = nullptr;
    const GLFWvidmode* mode = nullptr;

    const char *name;
    react::ReactiveStruct<glm::uvec2, 2> size;
    std::vector<std::string> icon;
    glm::ivec2 buffer_position;
    glm::ivec2 buffer_size;

    Window() = delete;
    ~Window() = default;
    Window(const Window &) = delete;
    Window &operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window &operator=(Window&&) = delete;

    Window(const glm::uvec2 &size, const char *name);

    void set_icon();
    void set_position(glm::uvec2 position);
    void set_full_screen(bool is_full_screen);

    glm::ivec2 get_position();

    static constexpr const char *get_type_name_imp() noexcept { return "Window"; }
};

}

#endif
