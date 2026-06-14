#include "comp/ECSWorld.hpp"

#include "res/ResourceManager.hpp"

#include "comp/core/Transform.hpp"
#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/Animator.hpp"
#include "comp/rendering/StateAnimator.hpp"
#include "comp/ui/CursorFollower.hpp"

#include "util/get_files_from_folder.hpp"
#include "util/mtrs_message.hpp"
#include "util/hash.hpp"

#define FILE_READ(file, date) file.read(reinterpret_cast<char*>(&date), sizeof(date))

#define COMPONENT_TYPE \
X(Transform)\
X(Sprite)\
X(Animator)\
X(StateAnimator)\
X(CursorFollower)

namespace mtrs::comp
{

ECSWorld::ECSWorld(const std::string &executable_path,const std::string &scenes_path)
{
    auto files = util::get_files_from_folder(scenes_path, ".mtsc");
    _scenes.reserve(files.size());
    for(auto &file : files)
    {
        _scenes.emplace(file, Scene{std::ifstream(), false});
    }
}

ECSWorld::ECSWorld(ECSWorld &&other) noexcept
{
    _executable_path = std::move(other._executable_path);
    _scenes = std::move(other._scenes);
}

ECSWorld &ECSWorld::operator=(ECSWorld &&other) noexcept
{
    if(this != &other)
    {
        _executable_path = std::move(other._executable_path);
        _scenes = std::move(other._scenes);
    }
    return *this;
}

ECSWorld::~ECSWorld()
{
    for(auto &scene : _scenes)
    {
        if(scene.second.file.is_open())
            scene.second.file.close();
    }
}

void ECSWorld::load_scene(std::string scene, mtrs::res::ResourceManager& resource, bool is_turn_on)
{
    auto iter = _scenes.find(scene);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        util::mtrs_message(util::TipeMessage::ERROR,
            "Failed to load scene, there is no scene named \"", scene,"\" in the list");
        return;
    }

    if(iter->second.init || iter->second.file.is_open())
    {
        util::mtrs_message(util::TipeMessage::LOG,
            "Failed to load scene, scene \"",scene,"\" is already loaded");
        return;
    }
#endif

    auto &file = iter->second.file;
    file.open(scene, std::ios::binary);

#ifndef FLAG_RELEASE
    char magic[4];
    file.read(magic, sizeof(magic));

    char true_magic[4] = {'m','t','s','c'};
    for(uint8_t i{}; i < 4; i++)
    {
        if(magic[i] != true_magic[i])
        {
            util::mtrs_message(util::TipeMessage::ERROR,
                "Failed to load scene, scene \"",scene,"\" does not have the magic .mtsc");
            file.close();
            return;
        }
    }
#endif

    file.seekg(8, std::ios::beg);

    uint32_t entity_count = 0;
    FILE_READ(file, entity_count);

    file.seekg(16, std::ios::beg);

    uint32_t index_offset;
    uint32_t data_offset;
    uint32_t free_date_offset;
    uint32_t dynamic_date_offset;

    FILE_READ(file, index_offset);
    FILE_READ(file, data_offset);
    FILE_READ(file, free_date_offset);
    FILE_READ(file, dynamic_date_offset);

    file.seekg(index_offset, std::ios::beg);

    struct Entity
    {
        uint64_t id;
        uint32_t offset;
        uint32_t size;
        EntityID engine_id;
    };
    Entity *entities = new Entity[entity_count];
    
    for(int i = 0; i < entity_count; i++)
    {
        FILE_READ(file, entities[i].id);
        FILE_READ(file, entities[i].offset);
        FILE_READ(file, entities[i].size);
        _components.create_entity(std::to_string(entities[i].id));
        entities[i].engine_id = _components.get_entity_by_name(std::to_string(entities[i].id));
    }

    if(!is_turn_on)
    {
        for(int i = 0; i < entity_count; i++)
        {
            _components.turn_off(entities[i].engine_id);
        }
    }
    
#ifndef FLAG_RELEASE
    if(file.tellg() != std::streampos(data_offset))
    {
        util::mtrs_message(util::TipeMessage::ERROR,
            "the cursor is at position ", data_offset, " for the name \"entity_data_offset\",",
            "but the document indicates position ", file.tellg());
        file.close();
        return;
    }
#endif

    uint64_t id_comp;
    uint32_t size;
    for(int i = 0; i < entity_count; i++)
    {
        FILE_READ(file, id_comp);
        FILE_READ(file,id_comp);
        switch (id_comp)
        {
#define X(Comp) case hash_c_string(#Comp): \
_components.add_comp(entities[i].engine_id, Comp(entities[i].engine_id, file, *this, resource)); \
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

}

void ECSWorld::remove_scene(std::string scene)
{

}

void ECSWorld::turn_on_scene(std::string scene)
{
    
}

void ECSWorld::turn_off_scene(std::string scene)
{

}

ComponentManager &ECSWorld::component_manager()
{
    return _components;
}


}