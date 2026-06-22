#include "Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/Audio.hpp"
#include "comp/single/States.hpp"
#include "comp/single/Cursor.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"

#include "res/asset/RenderContext.hpp"
#include "res/asset/Texture.hpp"
#include "sys/rendering/SpriteRenderSystem.hpp"

#include "util/mtrs_message.hpp"

namespace mtrs::engine
{

Core::Core(const Config& config)
: resources(config.executable_path, config.resurce_path)
, world(config.executable_path, config.scenes_path)
{
    window = world.single_comp<comp::Window>(config.size_in_pixels * config.start_pixel_scale, config.name_window.c_str());
    
    states = world.single_comp<comp::States>(config.init_state);

    if (!glfwInit())
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Failed GLFW init");
        _is_init = false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const glm::uvec2 &window_size = window->size.get();

    window->poiter = glfwCreateWindow(window_size.x, window_size.y, window->name, nullptr, nullptr);

    if (!window->poiter)
    {
        const char *description;
        int code = glfwGetError(&description);

        if (description)
        {
            util::mtrs_message(util::TipeMessage::ERROR, "Failed to create GLFW window: ", code);
        }
        
        glfwTerminate();
        _is_init = false;
    }
    glfwMakeContextCurrent(window->poiter);

    glfwSetWindowUserPointer(window->poiter, this);

    glfwSetFramebufferSizeCallback(window->poiter, window_size_callback);
    glfwSetKeyCallback(window->poiter, key_callback);
    glfwSetMouseButtonCallback(window->poiter, mouse_button_callback);
    glfwSetCursorPosCallback(window->poiter, cursor_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Couidn't load opengl");
        glfwTerminate();
        _is_init = false;
    }

    glfwSwapInterval(0);

    camera = world.single_comp<comp::Camera>(window->size, config.size_in_pixels);
    cursor = world.single_comp<comp::Cursor>(window->size, camera->size_in_pixels, camera->offset_viewport);
    keyboard = world.single_comp<comp::KeyButtons>(nullptr);
    mouse = world.single_comp<comp::MouseButtons>(nullptr);

    util::mtrs_message(util::TipeMessage::LOG, "Renderer: ", glGetString(GL_RENDERER));
    util::mtrs_message(util::TipeMessage::LOG, "OpenGL version: ", glGetString(GL_VERSION));
    util::mtrs_message(util::TipeMessage::LOG, "GLSL Version: ",
        glGetString(GL_SHADING_LANGUAGE_VERSION));


    comp::Audio *audio = world.single_comp<comp::Audio>(nullptr);
    if(audio)
    {
        audio->sound_scale = config.saund_location_scale;
        if(!audio->initialized)
        {
            util::mtrs_message(util::TipeMessage::ERROR, "Failed to init sound engine");
            glfwTerminate();
            _is_init = false;
        }
    }
    
    if(!config.display_cursor) glfwSetInputMode(window->poiter, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glClearColor(config.clear_color[0], config.clear_color[1], config.clear_color[2], config.clear_color[3]);
    
    if(config.depth) glEnable(GL_DEPTH_TEST);

    sys::SpriteRenderSystem::context = resources.get_resource<res::RenderContext>("system/render_context");
    
    _is_init = true;
}

void Core::garbage_collection()
{
    world.remove_marked();
    resources.garbage_collector();
}

void Core::pre_update()
{
    const glm::uvec2 &window_size = window->size.get();
    glfwSetWindowSize(window->poiter, window_size.x, window_size.y);
    window_size_callback(window->poiter, window_size.x, window_size.y);
}

void Core::update(float delta)
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.update(world, delta, states->current_system_state);

    world.remove_marked();

    glfwSwapBuffers(window->poiter);
}

void Core::shutdown()
{
    sys::SpriteRenderSystem::context.reset();
    world.clear_all();
    resources.garbage_collector();
    glfwTerminate();
}

bool Core::is_close_window()
{
    return glfwWindowShouldClose(window->poiter);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->window->size.set({width, height});
        core->camera->offset_viewport.update();
        core->camera->update_proj_matrix();
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->keyboard->keys[key] = action;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->mouse->buttons[button] = action;
    }
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->cursor->window_position.set({xpos, ypos});
        core->cursor->position.update();
    }
}

}





