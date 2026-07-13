#include "comp/single/Window.hpp"

#define  STBI_ONLY_PNG
#define  STBI_ONLY_JPEG
#include "stb_image.h"

#include "util/mtrs_message.hpp"

namespace mtrs::comp
{

Window::Window(const glm::uvec2 &size, const char *name)
: size(size), name(name)
, monitor(glfwGetPrimaryMonitor())
, mode(glfwGetVideoMode(monitor))
, buffer_position(0)
{

}

void Window::set_icon()
{
    GLFWimage icon_window[icon.size()];

    int channels;
    for(size_t i = 0; i < icon.size(); i++)
    {
        auto s = icon[i].substr(icon[i].length()-4);
        if(icon[i].substr(icon[i].length()-4) == ".png")
        {
            channels = 4;
        }
        else
        {
            channels = 3;
        }
        icon_window[i].pixels = stbi_load(icon[i].c_str(), &icon_window[i].width, &icon_window[i].height, &channels, 0);

        if(!icon_window[i].pixels)
        {
            util::mtrs_error("Failed to load icon: ", icon[i]);
            return;
        }
    }

    glfwSetWindowIcon(poiter, icon.size(), icon_window);
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
        glfwGetWindowPos(poiter, &buffer_position.x, &buffer_position.y);
        glfwGetWindowSize(poiter, &buffer_size.x, &buffer_size.y);
        glfwSetWindowMonitor(poiter, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(poiter, NULL, buffer_position.x, buffer_position.y,
            buffer_size.x, buffer_size.y, GLFW_DONT_CARE);
    }
}

glm::ivec2 Window::get_position()
{
    glm::ivec2 pos;
    glfwGetWindowPos(poiter, &pos.x, &pos.y);
    return pos;
}

}