#ifndef GAME_H
#define GAME_H

#include <glm/vec2.hpp>

class GLFWwindow;
namespace RenderEngine
{
    class ShaderProgram;
}
class ResourceManager;
class GameState;

namespace sys
{
    class Str;
}

class Game
{
public:
    Game(const glm::vec2 &windowSize, ResourceManager *resourceManager, GameState *startState, GLFWwindow *window);
    
    void render();
    void update(const double delta);
    
    void set_window_size(const glm::uvec2 &windowSize);
    glm::uvec2 getWindowSize() { return _windowSize; }
    void update_viewport();
    
    void set_key(const int key, const int action) { _keys[key] = action; }
    void set_mouse_buttons(const int buttom, const int action) { _mouseButtons[buttom] = action; }
    void set_cursor(const glm::dvec2 &cursor) { _cursor = cursor; }
    
    unsigned int get_current_game_width() const;
    unsigned int get_current_game_height() const;
    
private:
    bool _keys[349];
    bool _mouseButtons[8];
    bool _lastKeys[349];
    bool _lastMouseButtons[8];
    glm::dvec2 _cursor;
    
    GLFWwindow *_window;
    glm::vec2 _windowSize;
    
    RenderEngine::ShaderProgram *_shaderProgram;
    ResourceManager *_resourceManager;
    
    GameState *_currentGameState;
    void request_processing(const sys::Str nameRrequest, void *data);
};

#endif
