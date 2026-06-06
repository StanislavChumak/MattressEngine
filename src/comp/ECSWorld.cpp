#include "comp/ECSWorld.hpp"

#include "util/get_files_from_folder.hpp"
#include "util/mtrs_message.hpp"

namespace mtrs::comp
{

ECSWorld::ECSWorld(const std::string &executable_path,const std::string &scenes_path)
{

}

ECSWorld::ECSWorld(ECSWorld &&other) noexcept
{
    _clear_sets = std::move(other._clear_sets);
    _clear_singletons = std::move(_clear_singletons);

    _remove_entity = std::move(other._remove_entity);
    _destroy_queue = std::move(other._destroy_queue);

    _entity_index = other._entity_index;
    other._entity_index = 0;
    _names_entity = std::move(other._names_entity);
    _freed_ids = std::move(other._freed_ids);

    _executable_path = std::move(other._executable_path);
    _scenes = std::move(other._scenes);
}

ECSWorld &ECSWorld::operator=(ECSWorld &&other) noexcept
{
    if(this != &other)
    {
        _clear_sets = std::move(other._clear_sets);
        _clear_singletons = std::move(_clear_singletons);

        _remove_entity = std::move(other._remove_entity);
        _destroy_queue = std::move(other._destroy_queue);

        _entity_index = other._entity_index;
        other._entity_index = 0;
        _names_entity = std::move(other._names_entity);
        _freed_ids = std::move(other._freed_ids);

        _executable_path = std::move(other._executable_path);
        _scenes = std::move(other._scenes);
    }
    return *this;
}

ECSWorld::~ECSWorld()
{
    clear_set();
    clear_singletons();
    for(auto &scene : _scenes)
    {
        if(scene.second.is_open())
            scene.second.close();
    }
}

void ECSWorld::open_scene(std::string scene)
{

}

void ECSWorld::load_scene(std::string scene)
{

}

void ECSWorld::remove_scene(std::string scene)
{

}

void ECSWorld::close_scene(std::string scene)
{

}

EntityID ECSWorld::create_entity(std::string name)
{
    EntityID id;
    if(_freed_ids.empty())
    {
        id = _entity_index;
        _entity_index++;
    }
    else
    {
        id = _freed_ids.top();
        _freed_ids.pop();
    }
    _names_entity.emplace(std::move(name), id);
    return id;
}

void ECSWorld::remove_entity(EntityID id)
{
    for(auto fun : _remove_entity)
        fun(id);
}

void ECSWorld::mark_destroy(EntityID id)
{
    _destroy_queue.push_back(id);
}

void ECSWorld::remove_marked()
{
    for(auto id : _destroy_queue)
    {
        remove_entity(id);
        _freed_ids.push(id);
    }

    _destroy_queue.clear();
}

EntityID ECSWorld::get_name_entity(std::string name)
{ 
    auto it = _names_entity.find(name);
    if(it != _names_entity.end())
    {
        return it->second;
    }
    util::mtrs_message(util::TipeMessage::ERROR, "Fatal find Entity to name: ", name);
    return NULL_ENTITY;
}

void ECSWorld::clear_set()
{
    for(auto fun : _clear_sets)
        fun();
}

void ECSWorld::clear_singletons()
{
    for(auto fun : _clear_singletons)
        fun();
}

}