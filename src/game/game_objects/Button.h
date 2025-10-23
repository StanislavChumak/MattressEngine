#ifndef BUTTON_H
#define BUTTON_H

#include "glm/vec2.hpp"

typedef unsigned char bate;

namespace RenderEngine
{
    class Sprite2D;
}

class Button
{
public:
    enum ButtonState
    {
        IDLE,
        HOVERED,
        PRESSED,
        HOLD,
        RELEASED
    };

    Button(glm::uvec2 position, glm::uvec2 size, bate mouseButton, RenderEngine::Sprite2D *sprite = nullptr);
    ~Button();

    void input(const bool (&mouseButtons)[8], const bool (&lastMouseButtons)[8], const glm::dvec2 &cursor);
    void render(const float &layer = 0.0f, const size_t &frameId = 0) const;

    ButtonState get_button_state() const { return _buttonState;}

    void set_position(const glm::uvec2 &position) { _position = position; }
    void set_size(const glm::uvec2 &size) { _size = size; }
    void set_mouse_button(bate mouseButton) { _mouseButton = mouseButton; }

private:
    ButtonState _buttonState;

    glm::uvec2 _position;
    glm::uvec2 _size;
    bate _mouseButton;

    RenderEngine::Sprite2D *_sprite;
};

#endif
