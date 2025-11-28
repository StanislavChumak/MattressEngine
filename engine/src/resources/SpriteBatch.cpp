#include "resources/SpriteBatch.h"

#include "glad/glad.h"

#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"

SpriteBatch::SpriteBatch(SpriteBatch &&other) noexcept
{
    _vao = std::move(other._vao);
    _instances = std::move(other._instances);
    for(u_char i = 0; i < BUFFER_COUNT; i++)
        _instanceVBO[i] = std::move(other._instanceVBO[i]);
    _shader = std::move(other._shader);
    _texture = std::move(other._texture);
}

SpriteBatch &SpriteBatch::operator=(SpriteBatch &&other) noexcept
{
    if(this != &other)
    {
        _vao = std::move(other._vao);
        _instances = std::move(other._instances);
        for(u_char i = 0; i < BUFFER_COUNT; i++)
            _instanceVBO[i] = std::move(other._instanceVBO[i]);
        _shader = std::move(other._shader);
        _texture = std::move(other._texture);
    }
    return *this;
}

SpriteBatch::~SpriteBatch()
{
    _instances.clear();
}

void SpriteBatch::beginBatch()
{
    size_t cacheCout = _instances.size();
    _instances.clear();
    _instances.reserve(cacheCout);
}

void SpriteBatch::submit(Transform *transform, Sprite2D *sprite)
{
    InstanceData date;
    glm::mat4 glob = transform->globalMatrix;

    date.position = glm::vec2(glob[3].x, glob[3].y);

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(glob[0].x, glob[0].y));
    scale.y = glm::length(glm::vec2(glob[1].x, glob[1].y));
    date.size = sprite->size * scale;

    date.rotation = std::atan2(glob[0].y, glob[0].x);

    date.lbUV = sprite->subTexture.lbVertex;
    date.rtUV = sprite->subTexture.rtVertex;
    date.color = sprite->color;
    date.layer = sprite->layer;

    _instances.push_back(date);
}

void SpriteBatch::endBatch()
{
    if(_instances.empty()) return;

    ++_currentBufferIndex %= BUFFER_COUNT;
    BufferObject &vbo = _instanceVBO[_currentBufferIndex];

    glNamedBufferSubData(vbo.id(), 0, _instances.size()*sizeof(InstanceData), _instances.data());
    vbo.bind();
    
    _vao.bind();
    _shader->use();
    _texture->bind();
    
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)_instances.size());
}

void SpriteBatch::flush()
{

}