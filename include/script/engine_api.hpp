#ifndef ENGINE_API_HPP
#define ENGINE_API_HPP

#ifdef SCRIPT
    #include "util/mtrs_message.hpp"
#else
    #include <cstdint>
#endif

#if defined(_WIN32)
    #include <windows.h>
    #define EXPORT __declspec(dllexport)
#else
    #include <dlfcn.h>
    #define EXPORT __attribute__((visibility("default")))
#endif

#include "glm/fwd.hpp"

namespace mtrs::comp
{
    class ECSWorld;
    typedef uint64_t EntityID;

    struct Window;
    struct Camera;
    struct KeyButtons;
    struct MouseButtons;
    struct MouseScroll;
    struct Cursor;
};

namespace mtrs::res
{
    class ResourceManager;

    class TextureAtlas;
    class ScriptFile;
};

namespace mtrs::util
{
    enum class TypeMessage;
}

namespace mtrs
{
    struct EngineAPI
    {
        comp::ECSWorld *world;
        res::ResourceManager *resource;
        const char *scene;

        // util
        void (*message)(mtrs::util::TypeMessage, const char *) = nullptr;

        // ECSWorld
        void *(*world_single_comp)(comp::ECSWorld*, const char *) = nullptr;
        void *(*world_component)(comp::ECSWorld*, const char *, comp::EntityID) = nullptr;
        comp::EntityID (*world_get_entity)(comp::ECSWorld*, const char *, uint64_t) = nullptr;
        void (*world_load_scene)(comp::ECSWorld*, res::ResourceManager*, const char *, bool) = nullptr;
        void (*world_remove_scene)(comp::ECSWorld*, const char*) = nullptr;
        void (*world_turn_on_scene)(comp::ECSWorld*, const char*) = nullptr;
        void (*world_turn_off_scene)(comp::ECSWorld*, const char*) = nullptr;

        // Window
        void (*window_set_icon)(comp::Window*) = nullptr;
        void (*window_set_position)(comp::Window*, glm::uvec2) = nullptr;
        void (*window_set_full_screen)(comp::Window*, bool) = nullptr;
        glm::ivec2 (*window_get_position)(comp::Window*) = nullptr;

        // Camera
        void (*camera_update_UBO)(comp::Camera*) = nullptr;
        void (*camera_update_proj_matrix)(comp::Camera*) = nullptr;
        void (*camera_update_view_matrix)(comp::Camera*) = nullptr;

        // KeyButtons
        void (*key_subscribe)(comp::KeyButtons*, int, bool, void(*)());
        void (*key_unsubscribe)(comp::KeyButtons*, int, bool, void(*)());

        // MouseButtons
        void (*mouse_subscribe)(comp::MouseButtons*, int, bool, void(*)());
        void (*mouse_unsubscribe)(comp::MouseButtons*, int, bool, void(*)());

        // MouseScroll
        void (*scroll_subscribe)(comp::MouseScroll*, void(*)());
        void (*scroll_unsubscribe)(comp::MouseScroll*, void(*)());

        // Cursor
        void (*cursor_subscribe)(comp::Cursor*, void(*)());
        void (*cursor_unsubscribe)(comp::Cursor*, void(*)());

        // ScriptFile
        void *(*script_get_symbol)(res::ScriptFile*, const char*) = nullptr;

        // TextureAtlas
        glm::vec4 (*atlas_get_sub_texture)(const res::TextureAtlas*, size_t) = nullptr;
    };
};

#ifdef SCRIPT

inline mtrs::EngineAPI *api;

typedef mtrs::comp::EntityID EntityID;

namespace mtrs
{
    template<typename Component>
    Component *get_single_comp()
    {
        return reinterpret_cast<Component*>(api->world_single_comp(api->world,
            Component::get_type_name()));
    }

    template<typename Component>
    Component *get_comp(comp::EntityID entity)
    {
        return reinterpret_cast<Component*>(api->world_component(api->world,
            Component::get_type_name(), entity));
    }

    comp::EntityID get_entity(const char *scene, uint64_t hash_entity)
    {
        return api->world_get_entity(api->world, scene, hash_entity);
    }

    void load_scene(const char *scene, bool is_turn_on = true)
    {
        api->world_load_scene(api->world, api->resource, scene, is_turn_on);
    }

    void remove_scene(const char *scene)
    {
        api->world_remove_scene(api->world, scene);
    }

    void turn_on_scene(const char *scene)
    {
        api->world_turn_on_scene(api->world, scene);
    }

    void turn_off_scene(const char *scene)
    {
        api->world_turn_off_scene(api->world, scene);
    }
}


namespace mtrs::util::detail
{

void show_message(TypeMessage tmsg, std::string&& message)
{
    api->message(tmsg, message.c_str());
}

}

#endif

#endif
