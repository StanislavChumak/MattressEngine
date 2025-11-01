#include "../include/Engine.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec2.hpp"

#include "rapidjson/error/en.h"

#include "../include/components/core/Input.h"
#include "../include/components/core/Game.h"
#include "../include/components/core/Window.h"
#include "../include/components/rendering/Camera.h"
#include "../include/components/audio/AudioEngine.h"

#include "../include/components/registerComponent.h"
#include "../include/components/core/Transform.h"
#include "../include/components/rendering/Sprite.h"
#include "../include/components/rendering/Animator.h"

#include "systems/rendering/RenderSystem.h"
#include "systems/rendering/AnimatorSystem.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

#include "resources/renderer/Renderer.h"

void window_size_callback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow *window, double xpos, double ypos);

Engine::Engine(const char *executablePath, glm::uvec2 gameSize, const char *windowName, unsigned int windowScale)
: _resource(executablePath)
{
    _world.addSingleComponent(Input{});
    _world.addSingleComponent(Camera{});
    _world.addSingleComponent(Game{gameSize});
    Window &window = _world.addSingleComponent(Window{gameSize * windowScale, windowName, windowScale});
    Audio& audio = _world.addSingleComponent(Audio{});

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

    audio.init();
    if(!audio.initialized)
    {
        std::cerr << "Failed to init sound engine" << std::endl;
        glfwTerminate();
    }
    
    auto userPointer = std::make_pair<InputSystem*, ECSWorld*>(&_input, &_world);
    glfwSetWindowUserPointer(window.window, &userPointer);
    // cursor
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    lastRenderer::set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    lastRenderer::set_depth_test(true);

    window_size_callback(window.window, window.size.x, window.size.y);
}

Engine::~Engine()
{
    _resource.clear();
    _world.getSingleComponent<Audio>().destroy();
    _world.clearComponentStorage<Sprite2D>();
    glfwTerminate();
}

bool Engine::loadJsonComponent(std::string pathJsonComponent)
{
    registerComponent<Transform>("Transform");
    registerComponent<Sprite2D>("Sprite2D");
    registerComponent<Animator>("Animator");

    std::string JSONstring = _resource.get_file_string(pathJsonComponent);

#   ifndef FLAG_RELEASE
    if (JSONstring.empty())
    {
        std::cerr << "No JSON resorces file!" << std::endl;
        return false;
    }
#   endif

    rapidjson::Document document;
    rapidjson::ParseResult parseResult = document.Parse(JSONstring.c_str());

#   ifndef FLAG_RELEASE
    if (!parseResult)
    {
        std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << '(' << parseResult.Offset() << ')' << std::endl;
        std::cerr << "In JSON file:" << pathJsonComponent << std::endl;
        return false;
    }
#   endif

    auto entitiesIterator = document.FindMember("entities");
    if (entitiesIterator != document.MemberEnd())
    {
        for(const auto &currentEntity : entitiesIterator->value.GetArray())
        {
            EntityID id = _world.createEntity();
            for (auto it = currentEntity.MemberBegin(); it != currentEntity.MemberEnd(); ++it)
            {
                const char* name = it->name.GetString();
                const rapidjson::Value& value = it->value;
                typeRegistry[name].addComponentFromJson(id, value, _world, _resource);
            }
        }
    }

    return true;
}

bool Engine::loadJsonSystem(std::string pathJsonSystem)
{
    _system.registerSystem<RenderSystem>(SystemPriority::RENDERING);
    _system.registerAlwaysRunSystem<RenderSystem>();
    _system.registerSystem<AnimatorSystem>(SystemPriority::ANIMATION);
    _system.registerAlwaysRunSystem<AnimatorSystem>();

    return true;
}

bool Engine::loadJsonResource(std::string pathJsonResource)
{
    _resource.load_JSON_resources(pathJsonResource);

    return true;
}

void Engine::lifeCycle()
{
    auto window = _world.getSingleComponent<Window>().window;

    double lastTime = glfwGetTime(), currentTime, delta;

    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;

        glfwPollEvents();
        lastRenderer::clear();

        _system.update(_world, delta, "");

        glfwSwapBuffers(window);

            
        lastTime = currentTime;
    }
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
        glm::mat4 projection = glm::ortho(0.f, static_cast<float>(game.size.x), 0.f, static_cast<float>(game.size.y), -100.f, 100.f);
        camera.projection = projection;
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
    if (pair && pair->first && pair->second)
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



