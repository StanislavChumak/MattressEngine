#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "glm/fwd.hpp"
#include "../../system/pair.h"
#include "../../system/String.h"

class GameState
{
public:
    virtual void render() const = 0;
    virtual void update(const double &delta) = 0;
    virtual void process_input(const bool (&keys)[349], const bool (&lastKeys)[349], const bool (&mouseButtons)[8], const bool (&lastMouseButtons)[8], const glm::dvec2 &cursor) = 0;

    virtual unsigned int get_state_width() const = 0;
    virtual unsigned int get_state_height() const = 0;

    pair<sys::Str, void*> flag;
};

#endif
