#include "../include/Engine.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec2.hpp"

#include "../include/components/core/Input.h"
#include "../include/components/core/Game.h"
#include "../include/components/core/Window.h"
#include "../include/components/rendering/Camera.h"

#include "resources/renderer/Renderer.h"

#include "systems/rendering/RenderSystem.h"
#include "systems/rendering/AnimatorSystem.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

void window_size_callback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow *window, double xpos, double ypos);

bool initSoundEngine();
void uninitSoundEngine();

Engine::Engine(const char *executablePath, const char *JSONPathResorse,
            glm::uvec2 gameSize, const char *windowName, unsigned int windowScale)
: _resorce(executablePath)
{
    _world.addSingleComponent(Input{});
    _world.addSingleComponent(Game{gameSize});
    Window &window = _world.addSingleComponent(Window{gameSize * windowScale, windowName, windowScale});


    if (!glfwInit())
    {
#ifndef FLAG_RELEASE
        std::cerr << "Failed GLFW" << std::endl;
#endif
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.window = glfwCreateWindow(window.size.x, window.size.y, window.name, nullptr, nullptr);
#ifndef FLAG_RELEASE
    if (!window.window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        const char *description;
        int code = glfwGetError(&description);

        if (description)
            std::cerr << code << description;

        glfwTerminate();
    }
#endif
    glfwMakeContextCurrent(window.window);

    glfwSetFramebufferSizeCallback(window.window, window_size_callback);
    glfwSetKeyCallback(window.window, keyCallback);
    glfwSetMouseButtonCallback(window.window, mouseButtonCallback);
    glfwSetCursorPosCallback(window.window, cursorCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
#ifndef FLAG_RELEASE
        std::cerr << "Couidn't load opengl" << std::endl;
        glfwTerminate();
#endif
    }

#ifndef FLAG_RELEASE
    std::cout << "Renderer: " << lastRenderer::get_renderer_string() << std::endl;
    std::cout << "OpenGL version: " << lastRenderer::get_renderer_string() << std::endl;
    std::cout << "GLSL Version: " << lastRenderer::get_GLSL_version_string() << std::endl;
#endif
        
    glfwSetWindowSize(window.window, window.size.x, window.size.y);

    if(initSoundEngine())
    {
        std::cerr << "Failed to init sound engine" << std::endl;
        glfwTerminate();
    }



    _resorce.load_JSON_resources("res/resources.json");

    _systems.registerSystem<RenderSystem>(SystemPriority::RENDERING ,_resorce);
    _systems.registerAlwaysRunSystem<RenderSystem>();
    _systems.registerSystem<AnimatorSystem>(SystemPriority::ANIMATION);
    _systems.registerAlwaysRunSystem<AnimatorSystem>();
    
    InputSystem inputSystem;
    auto userPointer = std::make_pair<InputSystem*, ECSWorld*>(&inputSystem, &_world);
    glfwSetWindowUserPointer(window.window, &userPointer);
    //cursor
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    lastRenderer::set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    lastRenderer::set_depth_test(true);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    std::pair<InputSystem*, ECSWorld*> *pair = static_cast<std::pair<InputSystem*, ECSWorld*>*>(glfwGetWindowUserPointer(window));
    if (pair)
    {
        ECSWorld &world = *pair->second;

        glm::uvec2 windowSize = glm::uvec2(width, height);
        Game game = world.getSingleComponent<Game>();
        world.getSingleComponent<Window>().size = windowSize;

        const float aspectRatio = static_cast<float>(game.size.x) / game.size.y;
        unsigned int viewPortWidth = windowSize.x;
        unsigned int viewPortHeight = windowSize.y;
        unsigned int viewPortOffsetLeft = 0;
        unsigned int viewPortOffsetBottom = 0;
        if (static_cast<float>(windowSize.x) / windowSize.y > aspectRatio)
        {
            viewPortWidth = static_cast<unsigned int>(windowSize.y * aspectRatio);
            viewPortOffsetLeft = (windowSize.x - viewPortWidth) / 2;
        }
        else
        {
            viewPortHeight = static_cast<unsigned int>(windowSize.x / aspectRatio);
            viewPortOffsetBottom = (windowSize.y - viewPortHeight) / 2;
        }

        Camera &camera = world.getSingleComponent<Camera>();
        camera.setOffsetViewport(viewPortWidth, viewPortHeight, viewPortOffsetLeft, viewPortOffsetBottom);
        camera.projection = glm::ortho(0.f, static_cast<float>(game.size.x), 0.f, static_cast<float>(game.size.y), -100.f, 100.f);
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    std::pair<InputSystem*, ECSWorld*> *pair = static_cast<std::pair<InputSystem*, ECSWorld*>*>(glfwGetWindowUserPointer(window));
    if (pair)
        pair->first->setKey(*pair->second, key, (Input::Action)action);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    std::pair<InputSystem*, ECSWorld*> *pair = static_cast<std::pair<InputSystem*, ECSWorld*>*>(glfwGetWindowUserPointer(window));
    if (pair)
        pair->first->setMouseButton(*pair->second, button, (Input::Action)action);
}

void cursorCallback(GLFWwindow *window, double xpos, double ypos)
{
    std::pair<InputSystem*, ECSWorld*> *pair = static_cast<std::pair<InputSystem*, ECSWorld*>*>(glfwGetWindowUserPointer(window));
    if (pair)
    {
        InputSystem &input = *pair->first;
        ECSWorld &world = *pair->second;
        glm::dvec2 offset(world.getSingleComponent<Camera>().offsetViewport);
        glm::dvec2 pos(xpos, ypos);
        pos -= offset;
        unsigned int gameHeight = world.getSingleComponent<Game>().size.y;
        auto window = world.getSingleComponent<Window>();
        double ratio = (static_cast<double>(window.size.y - offset.y * 2) / window.scale) / gameHeight;
        pos /= window.scale * ratio;
        pos.y = gameHeight - pos.y;

        input.setCursor(world, pos);
    }
}



