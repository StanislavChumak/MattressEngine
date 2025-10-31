#ifndef ENGINE_H
#define ENGINE_H

#include "../src/ECS.h"
#include "../src/systems/SystemManager.h"
#include "../src/systems/input/InputSystem.h"
#include "../src/resources/ResourceManager.h"

#include "glm/fwd.hpp"

class Engine
{
private:
    ECSWorld _world;
    SystemManager _systems;
    InputSystem _input;
    ResourceManager _resorce;
public:
    Engine(const char *executablePath, const char *JSONPathResorse,
            glm::uvec2 gameSize, const char *nameWindow, unsigned int windowScale = 1);
};

#endif
