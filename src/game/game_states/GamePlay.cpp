#include "GamePlay.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite2D.h"
#include "../../sound/Sound.h"

#include "../game_objects/Button.h"

GamePlay::GamePlay()
: _backgroundSprite(ResourceManager::get_sprite2D("background"))
#define X(num) ,_##num##Sprite(ResourceManager::get_sprite2D(#num))
NUMBERS
#undef X
, _PoPSprite(ResourceManager::get_sprite2D("PoP"))
, _exitButton(new Button(glm::uvec2(BLOCK_SIZE * 4, BLOCK_SIZE * 5),glm::uvec2(BLOCK_SIZE / 2), GLFW_MOUSE_BUTTON_1,ResourceManager::get_sprite2D("exit")))
, _moveSound(ResourceManager::get_sound("move"))
, _isGameOver(false)
, _vectorMove(glm::ivec2(0))
, _emptyCells(15)
{
    for(bate i = 0; i < 4; i++) for(bate j = 0; j < 4; j++) _field[i][j] = 0;

    srand(_field[5][6]);
    flag.first = "";


    long rand = random();
    _field[rand % 4][random() % 4] = (rand % 2 + 1) * 2;

}

void GamePlay::render() const
{
    _backgroundSprite->render(glm::vec2(0) , glm::vec2(GAME_WIDTH, GAME_HEIGTH), 0.f, -1.f);
    
    _exitButton->render();

    for(bate i = 0; i < 4; i++) for(bate j = 0; j < 4; j++)
    {
        switch (_field[j][i])
        {
#define X(num) case num: _##num##Sprite->render(glm::vec2(BLOCK_SIZE / 2 + BLOCK_SIZE * i, BLOCK_SIZE / 2 + BLOCK_SIZE * j), glm::vec2(BLOCK_SIZE), 0.f, 1.f); break;
NUMBERS
#undef X
        case 4096:
        _PoPSprite->render(glm::vec2(BLOCK_SIZE / 2 + BLOCK_SIZE * i, BLOCK_SIZE / 2 + BLOCK_SIZE * j), glm::vec2(BLOCK_SIZE), 0.f, 1.f);
            break;
        }
    }
}

void GamePlay::move_cells(bate x, bate y)
{
    if (_vectorMove.x == 1  && x == 3 ||
        _vectorMove.x == -1 && x == 0 ||
        _vectorMove.y == 1  && y == 3 ||
        _vectorMove.y == -1 && y == 0) return;
    while(!_field[y+_vectorMove.y][x+_vectorMove.x])
    {
        //printf("(%d,%d) -> (%d,%d) = %d\n",x,y,x+_vectorMove.x,y+_vectorMove.y,_field[y][x]);
        _field[y+_vectorMove.y][x+_vectorMove.x] = _field[y][x];
        _field[y][x] = 0;
        x += _vectorMove.x;
        y += _vectorMove.y;
        if (_vectorMove.x == 1  && x == 3 ||
        _vectorMove.x == -1 && x == 0 ||
        _vectorMove.y == 1  && y == 3 ||
        _vectorMove.y == -1 && y == 0) return;
    }
    if(_field[y][x] == _field[y+_vectorMove.y][x+_vectorMove.x])
    {
        //printf("(%d,%d) == (%d,%d) -> %d\n",x,y,x+_vectorMove.x,y+_vectorMove.y,_field[y+_vectorMove.y][x+_vectorMove.x]);
        _field[y+_vectorMove.y][x+_vectorMove.x] *= 2;
        _field[y][x] = 0;
        _emptyCells++;
    }
    
}

void GamePlay::update(const double &delta)
{
    if(_isGameOver || (!_vectorMove.x && !_vectorMove.y)) return;


    int a[4] = {_vectorMove.x>0?-1:4,_vectorMove.x?_vectorMove.x:-1,_vectorMove.y>0?-1:4,_vectorMove.y?_vectorMove.y:-1};
    for(char i = _vectorMove.x>0?3:0; i != a[0]; i -= a[1])
    for(char j = _vectorMove.y>0?3:0; j != a[2]; j -= a[3])
    {
        if(_field[j][i])move_cells(i, j);
    }

    long rand = random();
    if(_emptyCells)
    {
        bate cellNum = rand % _emptyCells;
        for(bate i = 0; i < 4; i++) for(bate j = 0; j < 4; j++)
        {
            if(!_field[j][i])
            {
                if(!cellNum){ _field[j][i] = (rand % 2 + 1) * 2; _emptyCells--;}
                cellNum--;
            }
        }

    }

    _vectorMove = glm::ivec2(0);
}


void GamePlay::process_input(const bool (&keys)[349], const bool (&lastKeys)[349], const bool (&mouseButtons)[8], const bool (&lastMouseButtons)[8], const glm::dvec2 &cursor)
{
    _exitButton->input(mouseButtons, lastMouseButtons, cursor);
    if (_exitButton->get_button_state() == Button::ButtonState::PRESSED)
    {
        flag.first = "window_close";
    }

    if(_isGameOver) return;

    if(keys[GLFW_KEY_UP]    && !lastKeys[GLFW_KEY_UP])    { _vectorMove = glm::ivec2(0,1); }
    if(keys[GLFW_KEY_DOWN]  && !lastKeys[GLFW_KEY_DOWN])  { _vectorMove = glm::ivec2(0,-1); }
    if(keys[GLFW_KEY_RIGHT] && !lastKeys[GLFW_KEY_RIGHT]) { _vectorMove = glm::ivec2(1,0); }
    if(keys[GLFW_KEY_LEFT]  && !lastKeys[GLFW_KEY_LEFT])  { _vectorMove = glm::ivec2(-1,0); }
}