#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <glm/vec2.hpp>

#include "../System/Singleton.h"

#include <vector>
#include <unordered_set>
#include <memory>
#include <functional>

class GameObject;

namespace Physics
{
    struct AABB
    {
        AABB(const glm::vec2 &_botomLeft, const glm::vec2 &_topRight)
        : botomLeft(_botomLeft), topRight(_topRight)
        {}
        glm::vec2 botomLeft;
        glm::vec2 topRight;
    };

    struct Collider
    {
        Collider(const glm::vec2 &_botomLeft, const glm::vec2 &_topRight, std::function<void(const GameObject&, const glm::vec2&)> _onCollisionCallback = {})
        : boundingBox(_botomLeft, _topRight)
        , onCollisionCallback(_onCollisionCallback)
        , isActive(true)
        {}
        Collider(const AABB &_boundingBox, std::function<void(const GameObject&, const glm::vec2&)> _onCollisionCallback = {})
        : boundingBox(_boundingBox)
        , onCollisionCallback(_onCollisionCallback)
        , isActive(true)
        {}

        AABB boundingBox;
        bool isActive;
        std::function<void(const GameObject&, const glm::vec2&)> onCollisionCallback;
    };

    class PhysicsEngine : public Singleton<PhysicsEngine>
    {
    public:
        void init();

        void update(const double &delta);
        static void addDynamicGameObject(GameObject* gameObject);

    private:
        bool hasIntersection(const Collider& colliders1, const glm::vec2 &position1, const Collider& colliders2, const glm::vec2 &position2);

        std::unordered_set<GameObject *> _dynamicObjects;
    };
}

#endif
