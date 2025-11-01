#include "Engine.h"

#include "glm/vec2.hpp"

#define BLOCK_SIZE 32
#define SCALE 2
#define GAME_WIDTH 12 * BLOCK_SIZE
#define GAME_HEIGTH 9 * BLOCK_SIZE


int main(int argc, char **argv)
{
    Engine engine(*argv, glm::vec2(GAME_WIDTH, GAME_HEIGTH), "Engine", SCALE);

    engine.loadJsonResource("res/resources.json");
    engine.loadJsonComponent("res/components.json");
    engine.loadJsonSystem("");

    engine.lifeCycle();


    engine.~Engine();

    return 0;
}