#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "../gameConstants.h"
#include "GameState.h"

namespace RenderEngine
{
    class Sprite2D;
}
class Button;
class Sound;

class StartScreen : public GameState
{
public:

    StartScreen();
    ~StartScreen() = default;
    
    virtual void render() const override;
    virtual void update(const double &delta) override;
    virtual void process_input(const bool (&keys)[349], const bool (&lastKeys)[349],
                               const bool (&mouseButtons)[8], const bool (&lastMouseButtons)[8],
                               const glm::dvec2 &cursor) override;

    unsigned int get_state_width() const override { return GAME_WIDTH; }
    unsigned int get_state_height() const override { return GAME_HEIGTH; }
private:

    RenderEngine::Sprite2D *_backgroundSprite;

    Button *_playButton;
    Button *_exitButton;
};

#endif
