#include "PhysicsEngine.h"

#include "../Game/GameObjects/GameObject.h"
#include "../Game/GameStates/Level.h"

namespace Physics
{
    void PhysicsEngine::init()
    {
    }

    void PhysicsEngine::update(const double &delta)
    {
        for (auto &currentDynamicObject : _dynamicObjects)
        {
            if (currentDynamicObject->getCurrentVelosity() > 0)
            {
                if (currentDynamicObject->getCurrentDirection().x != 0.f)
                {
                    currentDynamicObject->setPosition(glm::vec2(currentDynamicObject->getCurrentPosition().x, static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().y / 4.f + 0.5f) * 4.f));
                }
                else if (currentDynamicObject->getCurrentDirection().y != 0.f)
                {
                    currentDynamicObject->setPosition(glm::vec2(static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().x / 4.f + 0.5f) * 4.f, currentDynamicObject->getCurrentPosition().y));
                }

                const auto newPosition = currentDynamicObject->getCurrentPosition() + currentDynamicObject->getCurrentDirection() * static_cast<float>(currentDynamicObject->getCurrentVelosity() * delta);
                std::vector<GameObject *> objectToCheck = _currentLevel->getObjectsInArr(newPosition, newPosition + currentDynamicObject->getSize());

                const auto &colliders = currentDynamicObject->getColliders();
                bool hasCollision = false;

                glm::vec2 dynamicObjectCollisionDirection = currentDynamicObject->getCurrentDirection();
                glm::vec2 objectCollisionDirection = currentDynamicObject->getCurrentDirection() * -1.f;

                for(const auto &currentDynamicObjectCollider : colliders)
                {
                    for (const auto &currentObjectToCheck : objectToCheck)
                    {
                        const auto &collidersToCheck = currentObjectToCheck->getColliders();
                        if (currentObjectToCheck->collides(currentDynamicObject->getLayer()) && !collidersToCheck.empty())
                        {
                            for(const auto &currentObjectCollider : currentObjectToCheck->getColliders())
                            {
                                if(currentObjectCollider.isActive && hasIntersection(currentDynamicObjectCollider, newPosition, currentObjectCollider, currentObjectToCheck->getCurrentPosition()))
                                {
                                    hasCollision = true;
                                    if(currentObjectCollider.onCollisionCallback)
                                    {
                                        currentObjectCollider.onCollisionCallback(*currentDynamicObject, objectCollisionDirection);
                                    }
                                    if(currentDynamicObjectCollider.onCollisionCallback)
                                    {
                                        currentDynamicObjectCollider.onCollisionCallback(*currentObjectToCheck, dynamicObjectCollisionDirection);
                                    }
                                }
                            }
                        }
                    }
                }

                
                if (!hasCollision)
                {
                    currentDynamicObject->setPosition(newPosition);
                }
                else
                {
                    if (currentDynamicObject->getCurrentDirection().x != 0.f)
                    {
                        currentDynamicObject->setPosition(glm::vec2(static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().x / 4.f + 0.5f) * 4.f, currentDynamicObject->getCurrentPosition().y));
                    }
                    if (currentDynamicObject->getCurrentDirection().y != 0.f)
                    {
                        currentDynamicObject->setPosition(glm::vec2(currentDynamicObject->getCurrentPosition().x, static_cast<unsigned int>(currentDynamicObject->getCurrentPosition().y / 4.f + 0.5f) * 4.f));
                    }
                    currentDynamicObject->onCollision();
                }
            }
        }
    }

    void PhysicsEngine::addDynamicGameObject(GameObject* gameObject)
    {
        loadInstance()._dynamicObjects.insert(std::move(gameObject));
    }

    void PhysicsEngine::setCurrentLevel(Level *currentLevel)
    {
        _currentLevel = std::move(currentLevel);
        _dynamicObjects.clear();
        _currentLevel->initLevel();
    }

    bool PhysicsEngine::hasIntersection(const Collider &collider1, const glm::vec2 &position1, const Collider &collider2, const glm::vec2 &position2)
    {
        const glm::vec2 colliderBottomLeftWorld1 = collider1.boundingBox.botomLeft + position1;
        const glm::vec2 colliderTopRightWorld1 = collider1.boundingBox.topRight + position1;

        const glm::vec2 colliderBotomLeftWorld2 = collider2.boundingBox.botomLeft + position2;
        const glm::vec2 colliderTopRightWorld2 = collider2.boundingBox.topRight + position2;

        if (colliderBottomLeftWorld1.x >= colliderTopRightWorld2.x)
        {
            return false;
        }
        if (colliderTopRightWorld1.x <= colliderBotomLeftWorld2.x)
        {
            return false;
        }
        if (colliderBottomLeftWorld1.y >= colliderTopRightWorld2.y)
        {
            return false;
        }
        if (colliderTopRightWorld1.y <= colliderBotomLeftWorld2.y)
        {
            return false;
        }
        return true;
    }
}