#include"system/init.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/Renderer.h"
#include "sound/Sound.h"
#include "resources/ResourceManager.h"
#include "game/Game.h"
#include "game/game_states/GamePlay.h"


int main(int argc, char **argv)
{
    GLFWwindow *window = nullptr;
    glm::uvec2 windowSize = glm::uvec2(GAME_WIDTH * SCALE, GAME_HEIGTH * SCALE);
    
    if(!init(window, windowSize, "2048"))
    {
        return -1;
    }
    
    ResourceManager *resourceManager = new ResourceManager(*argv);
    resourceManager->load_JSON_resources("res/resources.json");
    
    GameState *startScreen = new GamePlay();
    Game *game = new Game(windowSize, resourceManager, startScreen, window);
    glfwSetWindowUserPointer(window, game);
    
    RenderEngine::Renderer::set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
    RenderEngine::Renderer::set_depth_test(true);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // update
        glfwPollEvents();

        double delta = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        game->update(delta);
        
        // render
        RenderEngine::Renderer::clear();
        game->render();

        glfwSwapBuffers(window);
    }

    resourceManager->clear();

    Sound::uninit_engine();

    glfwTerminate();

    return 0;
}