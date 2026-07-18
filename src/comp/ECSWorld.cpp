#include "comp/ECSWorld.hpp"

#include "res/ResourceManager.hpp"

#include "comp/core/ScriptUpdate.hpp"
#include "comp/core/ScriptCallback.hpp"
#include "comp/core/Transform.hpp"

#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/Animator.hpp"
#include "comp/rendering/StateAnimator.hpp"
#include "comp/rendering/SpriteMap.hpp"
#include "comp/rendering/MapAnimator.hpp"

#include "comp/audio/SoundPlayer.hpp"

#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/States.hpp"
#include "comp/single/Audio.hpp"
#include "comp/single/Listener.hpp"
#include "comp/single/Cursor.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"

#include "util/files/get_folder.hpp"
#include "util/mtrs_message.hpp"
#include "util/hash.hpp"

#include "comp_struct/comp_type.def"

#define SINGLE_COMPONENT_TYPE \
X(Window)\
X(Camera)\
X(States)\
X(Audio)\
X(Listener)\
X(Cursor)\
X(KeyButtons)\
X(MouseButtons)\
X(MouseScroll)

#define FILE_READ(file, date) file.read(reinterpret_cast<char*>(&date), sizeof(date))

namespace mtrs::comp
{

ECSWorld::ECSWorld(const std::string &executable_path,const std::string &scenes_path)
: _scenes_path(scenes_path)
{
    auto files = file::get_files_from_folder(scenes_path, ".mtscn");
    _scenes.reserve(files.size());
    std::string name;
    for(auto &file : files)
    {
        name = file.substr(scenes_path.size(), (file.size() - scenes_path.size() - 6));
        _scenes.emplace(name, Scene{{}, std::ifstream(), false});
    }
}

ECSWorld::ECSWorld(ECSWorld &&other) noexcept
{
    _executable_path = std::move(other._executable_path);
    _scenes = std::move(other._scenes);
    _destroy_queue = std::move(other._destroy_queue);
}

ECSWorld &ECSWorld::operator=(ECSWorld &&other) noexcept
{
    if(this != &other)
    {
        _executable_path = std::move(other._executable_path);
        _scenes = std::move(other._scenes);
        _destroy_queue = std::move(other._destroy_queue);
    }
    return *this;
}

ECSWorld::~ECSWorld()
{
    for(auto &scene : _scenes)
    {
        if(scene.second.file.is_open())
        {
            scene.second.file.close();
        }
    }
}

void ECSWorld::load_scene(std::string scene, mtrs::res::ResourceManager& resource, bool is_turn_on)
{
    auto iter = _scenes.find(scene);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        util::mtrs_error("Failed to load scene,",
            " there is no scene named \"", scene,"\" in the list");
        return;
    }

    if(iter->second.init)
    {
        util::mtrs_warning("Attempting to load already loaded scene: ", scene);
        return;
    }
#endif

    auto &file = iter->second.file;
    std::string file_name = _scenes_path + scene + ".mtscn";
    if(file.is_open())
    {
        file.seekg(0, std::ios::beg);
    }
    else
    {
        file.open(file_name, std::ios::binary);
    }

#ifndef FLAG_RELEASE
    char magic[4];
    file.read(magic, sizeof(magic));

    char true_magic[4] = {'m','t','s','c'};
    for(uint8_t i{}; i < 4; i++)
    {
        if(magic[i] != true_magic[i])
        {
            util::mtrs_error("Failed to load scene,",
                " scene \"",scene,"\" does not have the magic .mtsc");
            file.close();
            return;
        }
    }
#endif

    file.seekg(8, std::ios::beg);

    uint32_t entity_count = 0;
    uint32_t data_offset;
    uint32_t free_date_offset;
    uint32_t dynamic_date_offset;
    
    FILE_READ(file, entity_count);
    FILE_READ(file, data_offset);
    FILE_READ(file, free_date_offset);
    FILE_READ(file, dynamic_date_offset);

#ifndef FLAG_RELEASE
    if(file.tellg() != data_offset)
    {
        util::mtrs_error("Failed to load scene,",
            " data_offset does not match the value ", data_offset);
        file.close();
        return;
    }
#endif

    for(int i = 0; i < entity_count; i++)
    {
        uint64_t id, offset;
        EntityID entity;
        FILE_READ(file, id);
        FILE_READ(file, offset);
        entity = _components.create_entity();
        iter->second.local_entities.emplace(id, entity);

        if(!is_turn_on)
        {
            _components.turn_off(entity);
        }

        uint64_t id_comp;
        while(file.tellg() != offset)
        {
            FILE_READ(file, id_comp);
            switch (id_comp)
            {
#define X(Comp) case util::hash_c_string<uint64_t>(#Comp): \
_components.add_comp<Comp>(entity, entity, scene.c_str(), file, *this, resource); \
break;
            COMPONENT_TYPE
#undef X
            default:
                util::mtrs_error("unknown component by id ", id_comp);
                file.close();
                return;
            }
        }
    }

#ifndef FLAG_RELEASE
    if(file.tellg() != std::streampos(free_date_offset))
    {
        util::mtrs_error("the cursor is at position ", free_date_offset,
            " for the name \"free_date_offset\",",
            "but the document indicates position ", file.tellg());
        file.close();
        return;
    }
#endif
    iter->second.init = true;
    iter->second.turn_on = is_turn_on;
    file.close();
}

void ECSWorld::remove_scene(std::string scene)
{
    auto iter = _scenes.find(scene);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        util::mtrs_error("Failed to remove scene,",
            "there is no scene named \"",scene,"\" in the list");
        return;
    }
#endif
    for(auto &entity : iter->second.local_entities)
    {
        _components.remove_entity(entity.second);
    }
    iter->second.local_entities.clear();
    iter->second.init = false;
}

void ECSWorld::turn_on_scene(std::string scene)
{
    auto iter = _scenes.find(scene);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        util::mtrs_error("Failed to turn on scene,",
            " there is no scene named \"",scene,"\" in the list");
        return;
    }
#endif
    for(auto &entity : iter->second.local_entities)
    {
        _components.turn_on(entity.second);
    }
    iter->second.turn_on = true;
}

void ECSWorld::turn_off_scene(std::string scene)
{
    auto iter = _scenes.find(scene);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        util::mtrs_error("Failed to turn off scene,",
            " there is no scene named \"",scene,"\" in the list");
        return;
    }
#endif
    _components.reserve_turn_off(iter->second.local_entities.size());
    for(auto &entity : iter->second.local_entities)
    {
        _components.turn_off(entity.second);
    }
    iter->second.turn_on = false;
}

void ECSWorld::mark_destroy(std::string name)
{
    _destroy_queue.push_back(util::hash_string<uint64_t>(name));
}

void ECSWorld::remove_marked()
{
    for(auto hash : _destroy_queue)
    {
        _components.remove_entity(hash);
    }

    _destroy_queue.clear();
}

void ECSWorld::clear_all()
{
    _components.clear_sets();
    _components.clear_singletons();

    for(auto &scene : _scenes)
    {
        if(scene.second.file.is_open())
        {
            scene.second.file.close();
        }
    }
    _scenes.clear();
}

void *ECSWorld::single_comp(const char *comp)
{
    switch(util::hash_c_string<uint64_t>(comp))
    {
#define X(Comp) case util::hash_c_string<uint64_t>(#Comp): \
return single_comp<Comp>();
        SINGLE_COMPONENT_TYPE
#undef X
    }
    return nullptr;
}

void *ECSWorld::component(const char *comp, EntityID entity)
{
    switch(util::hash_c_string<uint64_t>(comp))
    {
#define X(Comp) case util::hash_c_string<uint64_t>(#Comp): \
return component<Comp>(entity);
        COMPONENT_TYPE
#undef X
    }
    return nullptr;
}

EntityID ECSWorld::get_entity(const char *scene, uint64_t hash_entity)
{
    auto scene_iter = _scenes.find(scene);
#ifndef FLAG_RELEASE
    if(scene_iter == _scenes.end())
    {
        util::mtrs_error("Failed to find entity,",
            " there is no scene named \"",scene,"\" in the list");
        return NULL_ENTITY;
    }
#endif

    auto entity_iter = scene_iter->second.local_entities.find(hash_entity);
#ifndef FLAG_RELEASE
    if(entity_iter == scene_iter->second.local_entities.end())
    {
        util::mtrs_error("Failed to find entity,",
            " there is no entity with hash ",hash_entity," in the scene ",scene);
        return NULL_ENTITY;
    }
#endif

    return entity_iter->second;
}

}