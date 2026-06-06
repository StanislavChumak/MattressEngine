#include "Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "comp/single/Window.hpp"
#include "comp/single/Input.hpp"
#include "comp/single/States.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/Audio.hpp"
#include "comp/single/Cursor.hpp"

#include "comp/registerComponent.hpp"

#include "res/render/RenderContext.hpp"
#include "sys/rendering/SpriteRenderSystem.hpp"

#include "util/mtrs_message.hpp"

void window_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow *window, double xpos, double ypos);

namespace mtrs::engine
{

Core::Core(const Config& config)
: resources(config.executable_path, config.resurce_path),
world(config.executable_path, config.scenes_path),
input_system(world.add_single_comp(comp::Input{}))
{
    window = &world.add_single_comp(comp::Window(config.pixel_size * config.pixel_scale,
        config.name_window.c_str(), config.pixel_scale));
    states = &world.add_single_comp(comp::States{config.init_state});

    world.add_single_comp(comp::Cursor{});

    if (!glfwInit())
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Failed GLFW init");
        _is_init = false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->poiter = glfwCreateWindow(window->size.x, window->size.y, window->name, nullptr, nullptr);

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

    util::mtrs_message(util::TipeMessage::LOG, "Renderer: ", glGetString(GL_RENDERER));
    util::mtrs_message(util::TipeMessage::LOG, "OpenGL version: ", glGetString(GL_VERSION));
    util::mtrs_message(util::TipeMessage::LOG, "GLSL Version: ",
        glGetString(GL_SHADING_LANGUAGE_VERSION));

    comp::Camera &camera = world.add_single_comp(comp::Camera(config.pixel_size));

    comp::Audio *audio = world.get_single_comp<comp::Audio>();
    if(audio)
    {
        audio->sound_scale = config.saund_location_scale;
        audio->init();
        if(!audio->initialized)
        {
            util::mtrs_message(util::TipeMessage::ERROR, "Failed to init sound engine");
            glfwTerminate();
            _is_init = false;
        }
    }
    
    glfwSetWindowUserPointer(window->poiter, this);

    if(!config.display_cursor) glfwSetInputMode(window->poiter, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glClearColor(config.clear_color[0], config.clear_color[1], config.clear_color[2], config.clear_color[3]);
    
    if(config.depth) glEnable(GL_DEPTH_TEST);

    glfwSetWindowSize(window->poiter, window->size.x, window->size.y);
    window_size_callback(window->poiter, window->size.x, window->size.y);

    std::shared_ptr<res::RenderContext> context = std::make_shared<res::RenderContext>();
    resources.get_cache<res::RenderContext>()["context"] = context;
    sys::SpriteRenderSystem::context = std::move(context);
    
    _is_init = true;
}

void Core::garbage_collection()
{
    world.remove_marked();
    resources.garbage_collector();
}

void Core::pre_update()
{
    window_size_callback(window->poiter, window->size.x, window->size.y);
}

void Core::update(float delta)
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.update(world, delta, states->current_system_state);

    input_system.updateLastInput();
    world.remove_marked();

    glfwSwapBuffers(window->poiter);
}

void Core::shutdown()
{
    sys::SpriteRenderSystem::context.reset();
    world.~ECSWorld();
    resources.garbage_collector();
    glfwTerminate();
}

bool Core::is_close_window()
{
    return glfwWindowShouldClose(window->poiter);
}

}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        glm::uvec2 window_size = glm::uvec2(width, height);
        core->world.get_single_comp<mtrs::comp::Window>()->size = window_size;
        mtrs::comp::Camera *camera = core->world.get_single_comp<mtrs::comp::Camera>();

        const float aspect_ratio = static_cast<float>(camera->pixel_size.x) / camera->pixel_size.y;
        uint32_t view_width = window_size.x;
        uint32_t view_height = window_size.y;
        uint32_t view_offset_left = 0;
        uint32_t view_offset_bottom = 0;
        if (static_cast<float>(window_size.x) / window_size.y > aspect_ratio)
        {
            view_width = static_cast<uint32_t>(window_size.y * aspect_ratio);
            view_offset_left = (window_size.x - view_width) / 2;
        }
        else
        {
            view_height = static_cast<uint32_t>(window_size.x / aspect_ratio);
            view_offset_bottom = (window_size.y - view_height) / 2;
        }

        camera->set_offset_viewport(view_width, view_height, view_offset_left, view_offset_bottom);
        camera->update_proj_matrix();
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
        core->input_system.setKey(core->world, key, action);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
        core->input_system.setMouseButton(core->world, button, action);
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        mtrs::comp::Camera *camera = core->world.get_single_comp<mtrs::comp::Camera>();
        mtrs::comp::Window *window = core->world.get_single_comp<mtrs::comp::Window>();
        glm::dvec2 offset(camera->offset_viewport);
        glm::dvec2 pos(xpos, ypos);
        pos -= offset;
        uint32_t pixel_height = camera->pixel_size.y;
        double ratio = (static_cast<double>(window->size.y - offset.y * 2) / window->scale) / pixel_height;
        pos /= window->scale * ratio;
        pos.y = pixel_height - pos.y;

        core->input_system.setCursor(core->world, pos);
    }
}





