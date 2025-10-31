#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../../../renderer/Sprite2D.h"

struct Animator {
    unsigned short currentFrame = 0;
    double currentAnimationTime = 0;
    unsigned short framesCount = 0;
    double *framesDuration;
    Animator(unsigned short framesCount, double *framesDuration)
    : framesCount(framesCount), framesDuration(framesDuration) {}
};

#endif
