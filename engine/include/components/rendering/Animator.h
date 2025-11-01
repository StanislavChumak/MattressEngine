#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "rapidjson/document.h"

class ResourceManager;

struct Animator {
    unsigned short currentFrame = 0;
    double currentAnimationTime = 0;
    unsigned short framesCount;
    double *framesDuration;

    Animator() = default;

    void fromJson(const rapidjson::Value& j, const ResourceManager &resource)
    {
        auto jframesDuration = j["framesDuration"].GetArray();
        framesCount = jframesDuration.Size();
        framesDuration = new double[framesCount];
        for(int i = 0; i < framesCount; i++)
        {
            framesDuration[i] = jframesDuration[i].GetDouble();
        }
    }
};

#endif
