#include "res/render/SpriteBatch.hpp"

#include "glad/glad.h"

#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include <cstring>

namespace mtrs::res
{

SpriteBatch::SpriteBatch(SpriteBatch &&other) noexcept
{
    _vao = std::move(other._vao);
    _instances = std::move(other._instances);
    for(u_char i = 0; i < BUFFER_COUNT; i++)
    {
        _instance_vbo[i] = std::move(other._instance_vbo[i]);
        _mapped_buffers[i] = other._mapped_buffers[i];
        _fences[i] = other._fences[i];
        other._mapped_buffers[i] = nullptr;
        other._fences[i] = 0;
    }
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
        {
            _instance_vbo[i] = std::move(other._instance_vbo[i]);
            _mapped_buffers[i] = other._mapped_buffers[i];
            _fences[i] = other._fences[i];
            other._mapped_buffers[i] = nullptr;
            other._fences[i] = 0;
        }
        _shader = std::move(other._shader);
        _texture = std::move(other._texture);
    }
    return *this;
}

SpriteBatch::~SpriteBatch()
{
    _instances.clear();
}

void SpriteBatch::begin_batch()
{
    size_t cacheCout = _instances.size();
    _instances.clear();
    _instances.reserve(cacheCout);
}

void SpriteBatch::submit(InstanceData date)
{
    _instances.push_back(std::move(date));
}

void SpriteBatch::end_batch()
{
    if(_instances.empty()) return;

    _current_buffer_index++;
    _current_buffer_index %= BUFFER_COUNT;

    if(_fences[_current_buffer_index])
    {
        GLenum result = glClientWaitSync(_fences[_current_buffer_index], GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
        if (result != GL_ALREADY_SIGNALED && result != GL_CONDITION_SATISFIED)
        {
            glWaitSync(_fences[_current_buffer_index], 0, GL_TIMEOUT_IGNORED);
        }
        glDeleteSync(_fences[_current_buffer_index]);
        _fences[_current_buffer_index] = 0;
    }

    InstanceData *ptr_vbo = _mapped_buffers[_current_buffer_index];
    std::memcpy(ptr_vbo, _instances.data(), _instances.size() * sizeof(InstanceData));
    
    _vao.bind();
    _instance_vbo[_current_buffer_index].bind();
    
    _shader->use();
    _texture->bind();
    
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)_instances.size());

    _fences[_current_buffer_index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void SpriteBatch::flush()
{

}

}
