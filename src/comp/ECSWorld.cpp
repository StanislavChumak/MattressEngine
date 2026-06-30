#include "comp/ECSWorld.hpp"

#include "res/ResourceManager.hpp"

#include "comp/core/Transform.hpp"

#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/Animator.hpp"
#include "comp/rendering/StateAnimator.hpp"
#include "comp/rendering/SpriteMap.hpp"
#include "comp/rendering/MapAnimator.hpp"

#include "comp/audio/SoundPlayer.hpp"

#include "comp/ui/CursorFollower.hpp"

#include "util/get_files_from_folder.hpp"
#include "util/mtrs_message.hpp"
#include "util/hash.hpp"

#include "comp_struct/comp_type.def"

#define FILE_READ(file, date) file.read(reinterpret_cast<char*>(&date), sizeof(date))

namespace mtrs::comp
{

ECSWorld::ECSWorld(const std::string &executable_path,const std::string &scenes_path)
: _scenes_path(scenes_path)
{
    auto files = util::get_files_from_folder(scenes_path, ".mtsc");
    _scenes.reserve(files.size());
    std::string name;
    for(auto &file : files)
    {
        name = file.substr(scenes_path.size(), (file.size() - scenes_path.size() - 5));
        _scenes.emplace(name, Scene{std::ifstream(), false});
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

std::ifstream *ECSWorld::open_scene(const std::string &scene)
{
    auto iter = _scenes.find(scene);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        util::mtrs_message(util::TipeMessage::ERROR,
            "Failed to load scene, there is no scene named \"", scene,"\" in the list");
        return nullptr;
    }
#endif

    auto file = &iter->second.file;
    std::string file_name = _scenes_path + scene + ".mtsc";
    if(!file->is_open())
    {
        file->open(file_name, std::ios::binary);
    }

    file->seekg(0, std::ios::beg);

#ifndef FLAG_RELEASE
    char magic[4];
    file->read(magic, sizeof(magic));

    char true_magic[4] = {'m','t','s','c'};
    for(uint8_t i{}; i < 4; i++)
    {
        if(magic[i] != true_magic[i])
        {
            util::mtrs_message(util::TipeMessage::ERROR,
                "Failed to load scene, scene \"",scene,"\" does not have the magic .mtsc");
            file->close();
            return nullptr;
        }
    }
#endif

    return file;
}

void ECSWorld::load_scene(std::string scene, mtrs::res::ResourceManager& resource, bool is_turn_on)
{
    auto result = open_scene(scene);
    if(!result) return;
    if(_scenes[scene].init)
    {
        util::mtrs_message(util::TipeMessage::WARNING, "Attempting to load already loaded scene: ", scene);
        result->close();
        return;
    }

    std::ifstream &file = *result;

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
        util::mtrs_message(util::TipeMessage::ERROR,
            "Failed to load scene, data_offset does not match the value ", data_offset);
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
        entity = _components.create_entity(id);

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
_components.add_comp<Comp>(entity, entity, file, *this, resource); \
break;
            COMPONENT_TYPE
#undef X
            default:
                util::mtrs_message(util::TipeMessage::ERROR,
                    "unknown component by id ", id_comp);
                file.close();
                return;
            }
        }
    }

#ifndef FLAG_RELEASE
    if(file.tellg() != std::streampos(free_date_offset))
    {
        util::mtrs_message(util::TipeMessage::ERROR,
            "the cursor is at position ", free_date_offset, " for the name \"free_date_offset\",",
            "but the document indicates position ", file.tellg());
        file.close();
        return;
    }
#endif
    _scenes[scene].init = true;
    file.close();
}

void ECSWorld::remove_scene(std::string scene)
{
    auto result = open_scene(scene);
    if(!result) return;

    std::ifstream &file = *result;

    file.seekg(8, std::ios::beg);

    uint32_t entity_count = 0;
    uint32_t data_offset;

    FILE_READ(file, entity_count);
    FILE_READ(file, data_offset);

    file.seekg(data_offset, std::ios::beg);
    
    for(int i = 0; i < entity_count; i++)
    {
        uint64_t id, offset;
        EntityID entity;
        FILE_READ(file, id);
        FILE_READ(file, offset);

        _components.remove_entity(id);
        file.seekg(offset, std::ios::beg);
    }

    _scenes[scene].init = false;
}

void ECSWorld::turn_on_scene(std::string scene)
{
    auto result = open_scene(scene);
    if(!result) return;

    std::ifstream &file = *result;

    file.seekg(8, std::ios::beg);

    uint32_t entity_count = 0;
    uint32_t data_offset;

    FILE_READ(file, entity_count);
    FILE_READ(file, data_offset);

    file.seekg(data_offset, std::ios::beg);
    
    for(int i = 0; i < entity_count; i++)
    {
        uint64_t id, offset;
        EntityID entity;
        FILE_READ(file, id);
        FILE_READ(file, offset);

        _components.turn_on(id);
        file.seekg(offset, std::ios::beg);
    }
}

void ECSWorld::turn_off_scene(std::string scene)
{
    auto result = open_scene(scene);
    if(!result) return;

    std::ifstream &file = *result;

    file.seekg(8, std::ios::beg);

    uint32_t entity_count = 0;
    uint32_t data_offset;

    FILE_READ(file, entity_count);
    FILE_READ(file, data_offset);

    file.seekg(data_offset, std::ios::beg);
    
    for(int i = 0; i < entity_count; i++)
    {
        uint64_t id, offset;
        EntityID entity;
        FILE_READ(file, id);
        FILE_READ(file, offset);

        _components.turn_off(id);
        file.seekg(offset, std::ios::beg);
    }
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

}