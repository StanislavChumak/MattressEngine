#include "Engine.h"

#include "glm/vec2.hpp"

#define BLOCK_SIZE 32
#define SCALE 2
#define GAME_WIDTH 12 * BLOCK_SIZE
#define GAME_HEIGTH 9 * BLOCK_SIZE


int main(int argc, char **argv)
{
    Engine engine(*argv, "res/resources.json", glm::vec2(GAME_WIDTH,GAME_HEIGTH), "Engine", SCALE);

    

    
    
    

    

    // //-----------
    // EntityID field[9][12];
    // EntityID cursor;

    // for(int i = 0; i < 12; i++) for(int j = 0; j < 9; j++)
    // {
    //     field[j][i] = world.createEntity();
    //     world.addComponent(field[j][i], Transform{glm::ivec2(i * BLOCK_SIZE, j * BLOCK_SIZE), glm::ivec2(BLOCK_SIZE), 0.f});
    //     world.addComponent(field[j][i], Sprite2D{"grassland_cell", -1});
    //     world.addComponent(field[j][i], Animator(*resourceManager.get_sprite2D("grassland_cell")));
    // }

    // cursor = world.createEntity();
    // world.addComponent(cursor, Transform{glm::ivec2(BLOCK_SIZE), glm::ivec2(BLOCK_SIZE / 4), 0.f});
    // world.addComponent(cursor, Sprite2D{"cursor", 1});

    // std::function<void(glm::dvec2)> func = 
    // [&](glm::dvec2 pos) { world.getComponent<Transform>(cursor)->position = pos - glm::dvec2(0,BLOCK_SIZE / 4); };
    // inputSystem.cursorSubscribe(func);
    // // ------




    // double lastTime = glfwGetTime(), currentTime, delta;

    // while (!glfwWindowShouldClose(window.window))
    // {
    //     currentTime = glfwGetTime();
    //     delta = currentTime - lastTime;


    //     //----
    //     glfwPollEvents();
    //     RenderEngine::lastRenderer::clear();

    //     systemManager.update(world, delta, "");

    //     glfwSwapBuffers(window.window);
    //     //----

        
    //     lastTime = currentTime;
    // }

    // resourceManager.clear();

    // uninitSoundEngine();

    // glfwTerminate();

    return 0;
}