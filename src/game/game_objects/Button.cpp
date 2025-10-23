#include "Button.h"

#include "../../renderer/Sprite2D.h"

Button::Button(glm::uvec2 position, glm::uvec2 size, bate mouseButton, RenderEngine::Sprite2D *sprite)
: _position(position)
, _size(size)
, _mouseButton(mouseButton)
, _buttonState(Button::ButtonState::IDLE)
, _sprite(sprite)
{

}

Button::~Button()
{

}

void Button::input(const bool (&mouseButtons)[8], const bool (&lastMouseButtons)[8], const glm::dvec2 &cursor)
{
    if(_position.x < cursor.x && cursor.x < _position.x + _size.x &&
       _position.y < cursor.y && cursor.y < _position.y + _size.y)
    {
        if(mouseButtons[_mouseButton])
        {
            if(lastMouseButtons[_mouseButton])
            {
                _buttonState = Button::ButtonState::HOLD;
            }
            else
            {
                _buttonState = Button::ButtonState::PRESSED;
            }
        }
        else
        {
            if(lastMouseButtons[_mouseButton])
            {
                _buttonState = Button::ButtonState::RELEASED;
            }
            else
            {
                _buttonState = Button::ButtonState::HOVERED;
            }
        }
    }
    else
    {
        _buttonState = Button::ButtonState::IDLE;
    }
}

void Button::render(const float &layer, const size_t &frameId) const
{
    if(_sprite)
        _sprite->render(_position, _size, 0.f, layer, frameId);
}