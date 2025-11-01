#ifndef INPUT_H
#define INPUT_H

#include "rapidjson/document.h"
#include "glm/vec2.hpp"

struct  Input {
    enum Action {
        RELEASED,
        PRESED,
        HOLD
    };
    Action keys[349];
    Action mouseButtons[8];
    bool lastKeys[349];
    bool lastMouseButtons[8];
    glm::dvec2 cursor;
};

#endif
