#ifndef ENGINE_H
#define ENGINE_H

#include "../src/ECSWorld.h"
#include "../src/systems/SystemManager.h"
#include "../src/systems/input/InputSystem.h"
#include "../src/resources/ResourceManager.h"

#include "glm/fwd.hpp"

class Engine
{
private:
    ECSWorld _world;
    SystemManager _system;
    InputSystem _input;
    ResourceManager _resource;
public:
    Engine(const char *executablePath, glm::uvec2 gameSize, const char *nameWindow, unsigned int windowScale = 1);
    ~Engine();

    bool loadJsonComponent(std::string pathJsonComponent);
    bool loadJsonSystem(std::string pathJsonSystem);
    bool loadJsonResource(std::string pathJsonResource);

    void lifeCycle();

    ECSWorld &components() { return _world;}
};

#endif
