#include "comp/single/Window.hpp"

#define  STBI_ONLY_PNG
#define  STBI_ONLY_JPEG
#include "stb_image.h"

#include "util/func/mtrs_message.hpp"

namespace mtrs::comp
{

Window::Window(const glm::uvec2 &size, const char *name)
: size(size), name(name)
, _monitor(glfwGetPrimaryMonitor())
, _mode(glfwGetVideoMode(_monitor))
, _buffer_position(0)
{

}

void Window::set_icon(const char *const *paths, uint64_t count)
{
    GLFWimage icon_window[count];

    int channels;
    for(size_t i = 0; i < count; i++)
    {
        auto s = std::string(paths[i]);
        if(s.substr(s.length() - 4) == ".png")
        {
            channels = 4;
        }
        else
        {
            channels = 3;
        }
        icon_window[i].pixels = stbi_load(paths[i],
            &icon_window[i].width, &icon_window[i].height, &channels, 0);
#ifndef FLAG_RELEASE
        if(!icon_window[i].pixels)
        {
            util::mtrs_error("Failed to load icon: ", paths[i]);
            return;
        }
#endif
    }

    glfwSetWindowIcon(poiter, count, icon_window);
    for(auto &icon : icon_window)
    {
        stbi_image_free(icon.pixels);
    }
}

void Window::set_position(glm::uvec2 position)
{
    glfwSetWindowPos(poiter, position.x, position.y);
}

void Window::set_full_screen(bool is_full_screen)
{
    if(is_full_screen)
    {
        glfwGetWindowPos(poiter, &_buffer_position.x, &_buffer_position.y);
        glfwGetWindowSize(poiter, &_buffer_size.x, &_buffer_size.y);
        glfwSetWindowMonitor(poiter, _monitor, 0, 0, _mode->width, _mode->height, _mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(poiter, NULL, _buffer_position.x, _buffer_position.y,
            _buffer_size.x, _buffer_size.y, GLFW_DONT_CARE);
    }
}

glm::ivec2 Window::get_position()
{
    glm::ivec2 pos;
    glfwGetWindowPos(poiter, &pos.x, &pos.y);
    return pos;
}

}