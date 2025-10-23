#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/vec2.hpp>

//#include "../../Physics/PhysicsEngine.h"

#include <vector>

class GameObject
{
public:
    GameObject(const glm::vec2 &position, const glm::vec2 &size, const float &rotation, const float &layer);
    virtual ~GameObject();

    virtual void render() const =0;
    virtual void update(const double &delta){};

    virtual bool collides(const float layer) { return true; }
    virtual void onCollision() {};

    virtual glm::vec2 getCurrentPosition() const { return _position; }
    virtual glm::vec2 getSize() const { return _size; }
    virtual glm::vec2 getCurrentDirection() const { return _direction; }
    virtual float getCurrentRotation() const { return _rotation; }
    virtual double getCurrentVelosity() const { return _velosity; }
    virtual float getLayer() const { return _layer; }

    virtual void setVelosity(const double &velosity);
    virtual void setPosition(const glm::vec2 &position);

    //virtual std::vector<Physics::Collider>& getColliders() { return _colliders; }

protected:
    glm::vec2 _position;
    glm::vec2 _size;
    glm::vec2 _direction;
    float _rotation;
    double _velosity;
    float _layer;
    //std::vector<Physics::Collider> _colliders;
};

#endif