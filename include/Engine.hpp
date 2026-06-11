#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include "comp/ECSWorld.hpp"
#include "sys/SystemManager.hpp"
#include "sys/input/InputSystem.hpp"
#include "res/ResourceManager.hpp"

namespace mtrs
{

namespace comp
{
    struct Window;
    struct Camera;
    struct Cursor;
    struct States;
}

namespace engine
{
    struct Config
    {
        std::string executable_path;
        std::string resurce_path;
        std::string scenes_path;
        std::string name_window = "";
        unsigned int start_pixel_scale = 1;
        glm::uvec2 size_in_pixels;

        bool display_cursor = true;
        float clear_color[4] = {0.2f, 0.0f, 0.3f, 1.f};
        bool depth = true;

        std::string init_state;

        int saund_location_scale = 10;
    };

    void window_size_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void cursor_callback(GLFWwindow *window, double xpos, double ypos);

    class Core 
    {
        comp::Window *window;
        comp::Camera *camera;
        comp::Cursor *cursor;
        comp::States *states;

        bool _is_init;

        friend void window_size_callback(GLFWwindow *window, int width, int height);
        friend void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        friend void cursor_callback(GLFWwindow *window, double xpos, double ypos);

    public:
        res::ResourceManager resources;
        comp::ECSWorld world;
        sys::SystemManager systems;
        sys::InputSystem input_system;

        Core(const Config& config);
        bool is_init() { return _is_init; }
        void garbage_collection();
        void pre_update();
        void update(float delta);
        void shutdown();

        bool is_close_window();
    };
}

}

#endif
