#ifndef ENGINE_HPP
#define ENGINE_HPP

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
        unsigned int pixel_scale = 1;
        glm::uvec2 pixel_size;

        bool display_cursor = true;
        float clear_color[4] = {0.2f, 0.0f, 0.3f, 1.f};
        bool depth = true;

        std::string init_state;

        int saund_location_scale = 10;
    };

    class Core 
    {
        comp::Window *window;
        comp::States *states;

        bool _is_init;
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
