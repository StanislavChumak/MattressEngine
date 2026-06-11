#include "comp/ECSWorld.hpp"

#include "util/get_files_from_folder.hpp"

namespace mtrs::comp
{

ECSWorld::ECSWorld(const std::string &executable_path,const std::string &scenes_path)
{

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

ComponentManager &ECSWorld::component_manager()
{
    return _components;
}


}