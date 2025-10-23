#include "GameObject.h"

GameObject::GameObject(const glm::vec2 &position, const glm::vec2 &size, const float &rotation, const float &layer)
: _position(position), _size(size), _rotation(rotation), _layer(layer), _velosity(0), _direction(glm::vec2(0.f, 1.f))
{}

GameObject::~GameObject()
{}

void GameObject::setVelosity(const double &velosity)
{
    _velosity = velosity;
}

void GameObject::setPosition(const glm::vec2 &position)
{
    _position = position;
}