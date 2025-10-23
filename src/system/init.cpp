#include "init.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../renderer/Renderer.h"
#include "../sound/Sound.h"
#include "../game/Game.h"

#include "../game/gameConstants.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

void window_size_callback(GLFWwindow *window, int width, int height)
{
    Game *game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game)
        game->set_window_size(glm::vec2(width, height));
}

void input_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    Game *game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game)
    {
        glm::dvec2 offset(RenderEngine::Renderer::get_offset_viewport());
        glm::dvec2 pos(xpos, ypos);
        pos -= offset;
        int gameHeight = game->get_current_game_height();
        double windowScale = (static_cast<double>((game->getWindowSize().y) - offset.y * 2) / SCALE) / gameHeight;
        pos /= SCALE * windowScale;
        pos.y = gameHeight - pos.y;

        game->set_cursor(pos);
    }
}

void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Game *game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game)
        game->set_mouse_buttons(button, action);
}

void input_key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    Game *game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game)
        game->set_key(key, action);
}

bool init(GLFWwindow *&window, glm::uvec2 windowSize, const char *nameWindow)
{
    if (!glfwInit())
    {
#ifndef FLAG_RELEASE
        std::cerr << "Failed GLFW" << std::endl;
        return false;
#endif
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowSize.x, windowSize.y, nameWindow, nullptr, nullptr);
#ifndef FLAG_RELEASE
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        const char *description;
        int code = glfwGetError(&description);

        if (description)
            std::cerr << code << description;

        glfwTerminate();
        return false;
    }
#endif
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, input_key_callback);
    glfwSetMouseButtonCallback(window, input_mouse_button_callback);
    glfwSetCursorPosCallback(window, input_cursor_pos_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
#ifndef FLAG_RELEASE
        std::cerr << "Couidn't load opengl" << std::endl;
        glfwTerminate();
        return false;
#endif
    }

#ifndef FLAG_RELEASE
    std::cout << "Renderer: " << RenderEngine::Renderer::get_renderer_string() << std::endl;
    std::cout << "OpenGL version: " << RenderEngine::Renderer::get_renderer_string() << std::endl;
    std::cout << "GLSL Version: " << RenderEngine::Renderer::get_GLSL_version_string() << std::endl;
#endif
        
    glfwSetWindowSize(window, windowSize.x, windowSize.y);

    if(Sound::init_engine())
    {
        std::cerr << "Failed to init sound engine" << std::endl;
        glfwTerminate();
        return false;
    }

    return true;
}