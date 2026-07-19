#include "res/asset/ScriptFile.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"
#include "comp/single/Cursor.hpp"

#include "res/asset/TextureAtlas.hpp"

#include "util/files/data_mtrs_file.hpp"
#include "util/hash.hpp"
#include "util/mtrs_message.hpp"

#include "dynamic_field.def"
#include "res_struct/ScriptFile.struct"

namespace mtrs::res
{

ScriptFile::ScriptFile(ASSET_ARGS)
{
    ScriptFile_sc script_file;
    file.read(reinterpret_cast<char*>(&script_file), sizeof(script_file));

    std::string path;
    file::set_string_from_mtrs_file(file, path, DYNAMIC_ARGS(script_file, path));
    path = path.substr(0, path.rfind('.'));

    _handle = file::load_library(dir_resource + path + file::lib_extension());
    if (_handle)
    {
        _on_load = reinterpret_cast<decltype(_on_load)>(file::get_symbol(_handle, "on_load"));
        if(!_on_load) mtrs::util::mtrs_error(file::get_last_error());
        _on_unload = reinterpret_cast<decltype(_on_unload)>(file::get_symbol(_handle, "on_unload"));
        if(!_on_unload) mtrs::util::mtrs_error(file::get_last_error());
    }
    else
    {
        util::mtrs_error("Failed to load script at path: ", path, "\n",
            file::get_last_error());
    }
}

ScriptFile::ScriptFile(ScriptFile &&other) noexcept
{
    _handle = other._handle;
    other._handle = nullptr;
    _on_load = other._on_load;
    other._on_load = nullptr;
    _on_unload = other._on_unload;
    other._on_unload = nullptr;
}

ScriptFile &ScriptFile::operator=(ScriptFile &&other) noexcept
{
    if(this != &other)
    {
        _handle = other._handle;
        other._handle = nullptr;
        _on_load = other._on_load;
        other._on_load = nullptr;
        _on_unload = other._on_unload;
        other._on_unload = nullptr;
    }
    return *this;
}

ScriptFile::~ScriptFile()
{
    if(_handle)
    {
        _on_unload();
        file::free_library(_handle);
        _handle = nullptr;
    }
}

std::string ScriptFile::get_type_name_imp() noexcept
{
    return "scripts";
}

uint32_t ScriptFile::get_type_size_imp() noexcept
{
    return sizeof(ScriptFile_sc);
}

void ScriptFile::load(const char *scene, comp::EntityID entity,
    comp::ECSWorld& world, ResourceManager& resource)
{
    // base
    _api.world = &world;
    _api.resource = &resource;
    _api.scene = scene;

    // util
    _api.message = [](mtrs::util::TypeMessage tmsg, const char *msg)
        { util::detail::show_message(tmsg, msg); };

    // ECSWorld
    _api.world_single_comp = [](comp::ECSWorld*w, const char *c)
        { return w->single_comp(c); };
    _api.world_component = [](comp::ECSWorld*w, const char *c, comp::EntityID e)
        { return w->component(c, e); };
    _api.world_get_entity = [](comp::ECSWorld*w, const char *s, uint64_t h)
        { return w->get_entity(s,h); };
    _api.world_load_scene = [](comp::ECSWorld*w, res::ResourceManager*r, const char*s, bool is)
        { w->load_scene(s, *r, is); };
    _api.world_remove_scene = [](comp::ECSWorld*w, const char*s)
        { w->remove_scene(s); };
    _api.world_turn_on_scene = [](comp::ECSWorld*w, const char*s)
        { w->turn_on_scene(s); };
    _api.world_turn_off_scene = [](comp::ECSWorld*w, const char*s)
        { w->turn_off_scene(s); };

    // window
    _api.window_set_icon = [](comp::Window*w){ w->set_icon(); };
    _api.window_set_position = [](comp::Window*w, glm::uvec2 v){ w->set_position(v); };
    _api.window_set_full_screen = [](comp::Window*w, bool is){ w->set_full_screen(is); };
    _api.window_get_position = [](comp::Window*w){ return w->get_position(); };

    // camera
    _api.camera_update_UBO = [](comp::Camera*c){ c->update_UBO(); };
    _api.camera_update_proj_matrix = [](comp::Camera*c){ c->update_proj_matrix(); };
    _api.camera_update_view_matrix = [](comp::Camera*c){ c->update_view_matrix(); };

    // script file
    _api.script_get_symbol = [](res::ScriptFile *s, const char *n){ return s->get_symbol(n); };

    // texture atlas
    _api.atlas_get_sub_texture = [](const res::TextureAtlas *a, size_t i){ return glm::vec4(a->get_sub_texture(i)); };

    // key buttons
    _api.key_subscribe = [](mtrs::comp::KeyButtons *keybord, int key, bool action, void (*callback)())
        { keybord->subscribe(key, action, callback); };
    _api.key_unsubscribe = [](mtrs::comp::KeyButtons *keybord, int key, bool action, void (*callback)())
        { keybord->unsubscribe(key, action, callback); };

    // mouse buttons
    _api.mouse_subscribe = [](mtrs::comp::MouseButtons *mouse, int button, bool action, void (*callback)())
        { mouse->subscribe(button, action, callback); };
    _api.mouse_unsubscribe = [](mtrs::comp::MouseButtons *mouse, int button, bool action, void (*callback)())
        { mouse->unsubscribe(button, action, callback); };
    
    // mouse scroll
    _api.scroll_subscribe = [](mtrs::comp::MouseScroll *scroll, void (*callback)())
        { scroll->subscribe(callback); };
    _api.scroll_unsubscribe = [](mtrs::comp::MouseScroll *scroll, void (*callback)())
        { scroll->unsubscribe(callback); };
    
    // cursor
    _api.cursor_subscribe = [](mtrs::comp::Cursor *cursor, void (*callback)())
        { cursor->subscribe(callback); };
    _api.cursor_unsubscribe = [](mtrs::comp::Cursor *cursor, void (*callback)())
        { cursor->unsubscribe(callback); };

    _on_load(entity, &_api);
}

void *ScriptFile::get_symbol(std::string &&name)
{
    if(_handle)
    {
        return file::get_symbol(_handle, name);
    }
    mtrs::util::mtrs_error("attempt to get a ", name, " from a script that is not loaded");
    return nullptr;
}

}