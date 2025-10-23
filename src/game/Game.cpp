#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game_states/GameState.h"
#include "../resources/ResourceManager.h"
#include "../renderer/Renderer.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Texture2D.h"
#include "../renderer/Sprite2D.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

Game::Game(const glm::vec2 &windowSize, ResourceManager *resourceManager, GameState *startState, GLFWwindow *window)
: _resourceManager(resourceManager)
, _shaderProgram(ResourceManager::get_shader_program("shaderSprite2D"))
, _window(window)
, _currentGameState(startState)
{
    set_window_size(std::move(windowSize));

    for(int i = 0; i < 349; i++) _lastKeys[i] = _keys[i] = false;
    for(int i = 0; i < 8; i++) _lastMouseButtons[i] = _mouseButtons[i] = false;
}

void Game::render()
{
    _currentGameState->render();
}

void Game::update(const double delta)
{
    _currentGameState->process_input(_keys, _lastKeys, _mouseButtons, _lastMouseButtons, _cursor);
    _currentGameState->update(delta);

    request_processing(_currentGameState->flag.first, _currentGameState->flag.second);

    for(int i = 0; i < 349; i++) _lastKeys[i] = _keys[i];
    for(int i = 0; i < 8; i++) _lastMouseButtons[i] = _mouseButtons[i];
}

void Game::request_processing(const sys::Str nameRrequest, void *data)
{
    if(nameRrequest == "set_game_state")
    {
        GameState *newGameState = static_cast<GameState*>(data);
#ifndef FLAG_RELEASE
        if(!newGameState)
        {
            printf("Failed to process request: %s file:%s func:%s line:%d\n", nameRrequest, __FILE__, __func__, __LINE__);
            return;
        }
#endif
        for(int i = 0; i < 349; i++) _keys[i] = false;
        for(int i = 0; i < 8; i++) _mouseButtons[i] = false;
        delete _currentGameState;
        _currentGameState = newGameState;
        return;
    }

    if(nameRrequest == "window_close")
    {
        glfwSetWindowShouldClose(_window, true);
        return;
    }
}

void Game::set_window_size(const glm::uvec2 &windowSize)
{
    _windowSize = std::move(windowSize);
    update_viewport();
}

void Game::update_viewport()
{
    const float aspectRatio = static_cast<float>(get_current_game_width()) / get_current_game_height();
    unsigned int viewPortWidth = _windowSize.x;
    unsigned int viewPortHeight = _windowSize.y;
    unsigned int viewPortOffsetLeft = 0;
    unsigned int viewPortOffsetBottom = 0;
    if (static_cast<float>(_windowSize.x) / _windowSize.y > aspectRatio)
    {
        viewPortWidth = static_cast<unsigned int>(_windowSize.y * aspectRatio);
        viewPortOffsetLeft = (_windowSize.x - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = static_cast<unsigned int>(_windowSize.x / aspectRatio);
        viewPortOffsetBottom = (_windowSize.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::set_viewport(viewPortWidth, viewPortHeight, viewPortOffsetLeft, viewPortOffsetBottom);
    
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(get_current_game_width()), 0.f, static_cast<float>(get_current_game_height()), -100.f, 100.f);
    _shaderProgram->use();
    _shaderProgram->set_matrix4("projectionMatrix", projectionMatrix);
}

unsigned int Game::get_current_game_width() const
{
    return _currentGameState->get_state_width();
}

unsigned int Game::get_current_game_height() const
{
    return _currentGameState->get_state_height();
}