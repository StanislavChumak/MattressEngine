#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "GameState.h"
#include "../gameConstants.h"

#include <glm/vec2.hpp>

#define NUMBERS \
X(2) \
X(4) \
X(8) \
X(16) \
X(32) \
X(64) \
X(128) \
X(256) \
X(512) \
X(1024) \
X(2048)


typedef unsigned char bate;
typedef unsigned short ushotr;

namespace RenderEngine
{
    class Sprite2D;
}
class Button;
class Sound;

class GamePlay : public GameState 
{
public:

    GamePlay();

    void render() const override;
    void update(const double &delta) override;
    void process_input(const bool (&keys)[349], const bool (&lastKeys)[349], const bool (&mouseButtons)[8], const bool (&lastMouseButtons)[8], const glm::dvec2 &cursor) override;

    unsigned int get_state_width() const override { return GAME_WIDTH; }
    unsigned int get_state_height() const override { return GAME_HEIGTH; }

private:
    ushotr _field[4][4];
    bate _emptyCells;
    glm::ivec2 _vectorMove;

    void move_cells(bate x, bate y);

    bool _isGameOver;

    RenderEngine::Sprite2D *_backgroundSprite;

    RenderEngine::Sprite2D *_2Sprite;
    RenderEngine::Sprite2D *_4Sprite;
    RenderEngine::Sprite2D *_8Sprite;
    RenderEngine::Sprite2D *_16Sprite;
    RenderEngine::Sprite2D *_32Sprite;
    RenderEngine::Sprite2D *_64Sprite;
    RenderEngine::Sprite2D *_128Sprite;
    RenderEngine::Sprite2D *_256Sprite;
    RenderEngine::Sprite2D *_512Sprite;
    RenderEngine::Sprite2D *_1024Sprite;
    RenderEngine::Sprite2D *_2048Sprite;
    RenderEngine::Sprite2D *_PoPSprite;
    Button *_exitButton;

    Sound *_moveSound;
};

#endif
