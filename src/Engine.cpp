#include "Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/Audio.hpp"
#include "comp/single/States.hpp"
#include "comp/single/Cursor.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"

#include "res/asset/RenderContext.hpp"
#include "res/asset/Texture.hpp"

#include "sys/core/InputSystem.hpp"
#include "sys/rendering/SpriteRenderSystem.hpp"
#include "sys/rendering/SpriteMapRenderSystem.hpp"
#include "sys/rendering/CameraSystem.hpp"

#include "util/mtrs_message.hpp"

#include <thread>

namespace mtrs::engine
{

Core::Core(const Config& config)
: resources(config.executable_path, config.resurce_path)
, world(config.executable_path, config.scenes_path)
{
    if(!config.fixed_horizontal && !config.fixed_vertical)
    {
        util::mtrs_error("Only one side can be non-fixed, correct the configuration");
    }

    if (!glfwInit())
    {
        util::mtrs_error("Failed GLFW init");
        _is_init = false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = world.single_comp<comp::Window>(config.size_in_points * config.start_point_size, config.name_window.c_str());
    const glm::uvec2 &window_size = _window->size.get();

    _window->poiter = glfwCreateWindow(window_size.x, window_size.y, _window->name, nullptr, nullptr);

    if (!_window->poiter)
    {
        const char *description;
        int code = glfwGetError(&description);

        if (description)
        {
            util::mtrs_error("Failed to create GLFW window: ", code);
        }
        
        glfwTerminate();
        _is_init = false;
    }
    glfwMakeContextCurrent(_window->poiter);

    glfwSetWindowUserPointer(_window->poiter, this);

    glfwSetFramebufferSizeCallback(_window->poiter, window_size_callback);
    glfwSetKeyCallback(_window->poiter, key_callback);
    glfwSetMouseButtonCallback(_window->poiter, mouse_button_callback);
    glfwSetScrollCallback(_window->poiter, scroll_callback);
    glfwSetCursorPosCallback(_window->poiter, cursor_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        util::mtrs_error("Couidn't load opengl");
        glfwTerminate();
        _is_init = false;
    }

    glfwSwapInterval(0);

    _states = world.single_comp<comp::States>(config.init_state);
    _camera = world.single_comp<comp::Camera>(_window, glm::bvec2{config.fixed_horizontal,
        config.fixed_vertical}, config.size_in_points);
    _cursor = world.single_comp<comp::Cursor>(_camera);
    _keyboard = world.single_comp<comp::KeyButtons>(nullptr);
    _mouse = world.single_comp<comp::MouseButtons>(nullptr);
    _scroll = world.single_comp<comp::MouseScroll>(nullptr);

    util::mtrs_info("Renderer: ", glGetString(GL_RENDERER));
    util::mtrs_info("OpenGL version: ", glGetString(GL_VERSION));
    util::mtrs_info("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

    comp::Audio *audio = world.single_comp<comp::Audio>(nullptr);
    if(audio)
    {
        audio->sound_scale = config.saund_location_scale;
        if(!audio->is_init)
        {
            util::mtrs_error("Failed to init sound engine");
            glfwTerminate();
            _is_init = false;
        }
    }
    
    if(!config.display_cursor) glfwSetInputMode(_window->poiter, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glClearColor(config.clear_color[0], config.clear_color[1], config.clear_color[2], config.clear_color[3]);
    
    if(config.depth) glEnable(GL_DEPTH_TEST);

    sys::SpriteRenderSystem::context = resources.get_resource<res::RenderContext>("system/render_context");
    sys::SpriteMapRenderSystem::context = resources.get_resource<res::RenderContext>("system/render_context");

    sys::InputSystem::key_buttons = _keyboard;
    sys::InputSystem::mouse_buttons = _mouse;
    sys::InputSystem::mouse_scroll = _scroll;

    sys::CameraSystem::camera = _camera;

    _window->icon = config.icon_window;
    _window->set_icon();

    _time_frame = std::chrono::duration<double>(1.0 / config.max_fps);
    
    _is_init = true;
}

void Core::garbage_collection()
{
    world.remove_marked();
    resources.garbage_collector();
}

void Core::pre_update()
{
    const glm::uvec2 &window_size = _window->size.get();
    glfwSetWindowSize(_window->poiter, window_size.x, window_size.y);
    window_size_callback(_window->poiter, window_size.x, window_size.y);

    end = std::chrono::steady_clock::now();
}

void Core::update()
{
    start = end;

    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.update(world, _delta.count());

    world.remove_marked();

    glfwSwapBuffers(_window->poiter);

    end = std::chrono::steady_clock::now();
    _delta = end - start;
    if(_delta < _time_frame)
    {
        std::this_thread::sleep_for(_time_frame - _delta);
        end = std::chrono::steady_clock::now();
        _delta = end - start;
    }
}

void Core::shutdown()
{
    sys::SpriteRenderSystem::context.reset();
    sys::SpriteMapRenderSystem::context.reset();
    world.clear_all();
    resources.garbage_collector();
    glfwTerminate();
}

bool Core::is_close_window()
{
    return glfwWindowShouldClose(_window->poiter);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->_window->size.set({width, height});
        if(!core->_camera->fixed_sides.y)
        {
            core->_camera->size_in_points.set_field(&glm::uvec2::y,
                height / (width / (double)core->_camera->size_in_points->x));
        }
        else if(!core->_camera->fixed_sides.x)
        {
            core->_camera->size_in_points.set_field(&glm::uvec2::x,
                width / (height / (double)core->_camera->size_in_points->y));
        }
        core->_camera->viewport.update();
        core->_camera->update_proj_matrix();
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if(core)
    {
        core->_keyboard->keys[key] = action;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->_mouse->buttons[button] = action;
    }
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->_scroll->scroll += glm::dvec2{x_offset, y_offset};
    }
}

static glm::uvec2 _pos_buffer;
static glm::bvec2 _result_buffer;
void cursor_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        _pos_buffer = {x_pos, y_pos};
        // _result_buffer = glm::lessThan(_pos_buffer, core->_camera->viewport.get());
        // if(_result_buffer.x || _result_buffer.y) return;
        // _result_buffer = glm::lessThan(core->_window->size - core->_camera->viewport, _pos_buffer);
        // if(_result_buffer.x || _result_buffer.y) return;
        core->_cursor->window_position.set(std::move(_pos_buffer));
        core->_cursor->position.update();
        //MTRS_INFO(core->_camera->point_scale.get().x, " ", core->_camera->point_scale.get().y);
    }
}

}





