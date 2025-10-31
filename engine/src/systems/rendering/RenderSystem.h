#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "SystemBase.h"
#include "../../resources/ResourceManager.h"

class RenderSystem : public SystemBase
{
    ResourceManager &resorce;
public:
    RenderSystem(ResourceManager &resorce);

    void update(ECSWorld &world, const double &delta) override;
};

#endif
