#include "StartScreen.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/vec2.hpp"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite2D.h"
#include "../game_objects/Button.h"

#include "GamePlay.h"

#include "../../sound/Sound.h"

StartScreen::StartScreen()
: _backgroundSprite(ResourceManager::get_sprite2D("background"))
, _exitButton(new Button(glm::uvec2(BLOCK_SIZE * 10, BLOCK_SIZE * 13),glm::uvec2(BLOCK_SIZE),GLFW_MOUSE_BUTTON_1,ResourceManager::get_sprite2D("exit")))
, _playButton(new Button(glm::uvec2(BLOCK_SIZE * 4, BLOCK_SIZE * 10),glm::uvec2(BLOCK_SIZE * 4, BLOCK_SIZE * 2),GLFW_MOUSE_BUTTON_1,ResourceManager::get_sprite2D("play")))
{
    flag.first = "";
}

void StartScreen::render() const
{
    _backgroundSprite->render(glm::vec2(0) , glm::vec2(GAME_WIDTH, GAME_HEIGTH), 0.f, -1.f);

    _exitButton->render();
    _playButton->render();
}

void StartScreen::update(const double &delta)
{
}

void StartScreen::process_input(const bool (&keys)[349], const bool (&lastKeys)[349], const bool (&mouseButtons)[8], const bool (&lastMouseButtons)[8], const glm::dvec2 &cursor)
{
    _exitButton->input(mouseButtons, lastMouseButtons, cursor);
    if (_exitButton->get_button_state() == Button::ButtonState::PRESSED)
    {
        flag.first = "window_close";
    }

    _playButton->input(mouseButtons, lastMouseButtons, cursor);
    if (_playButton->get_button_state() == Button::ButtonState::PRESSED)
    {
        flag.first = "set_game_state";
        flag.second = new GamePlay;
    }
}