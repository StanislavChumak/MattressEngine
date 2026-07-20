#include "res/render/SpriteBatch.hpp"

#include "glad/glad.h"

#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include <cstring>

#ifndef FLAG_RELEASE
    #include "util/mtrs_message.hpp"
#endif

namespace mtrs::res
{

SpriteBatch::SpriteBatch(const BufferObject &quad_VBO, const BufferObject &quad_EBO,
    std::shared_ptr<const ShaderProgram> shader, std::shared_ptr<const Texture> texture)
{
    _shader = std::move(shader);
    _texture = std::move(texture);

    for(uint8_t i = 0; i < SpriteBatch::BUFFER_COUNT; i++)
    {
        const uint64_t max_instances = _texture->max_instances();
        _instance_vbo[i]._mode = GL_ARRAY_BUFFER;
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo[i]._id);
        glBufferStorage(GL_ARRAY_BUFFER, max_instances * sizeof(InstanceData), nullptr,
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

        _mapped_buffers[i] = reinterpret_cast<InstanceData*>(glMapBufferRange(
            GL_ARRAY_BUFFER, 0, max_instances * sizeof(InstanceData),
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));
    }

    _vao.set_vertex_buffer(0, quad_VBO, sizeof(float) * 4, 0);
    _vao.add_attrib_float(0, 0, 2, 0);
    _vao.add_attrib_float(1, 0, 2, sizeof(float)*2);

    _vao.set_vertex_buffer(1, _instance_vbo[0], sizeof(InstanceData), 0);
    _vao.add_attrib_float (2, 1, 2, offsetof(InstanceData, position));
    _vao.add_attrib_float (3, 1, 2, offsetof(InstanceData, size));
    _vao.add_attrib_float (4, 1, 1, offsetof(InstanceData, rotation));
    _vao.add_attrib_float (5, 1, 2, offsetof(InstanceData, lb_uv));
    _vao.add_attrib_float (6, 1, 2, offsetof(InstanceData, rt_uv));
    _vao.add_attrib_byteN (7, 1, 4, offsetof(InstanceData, color));
    glVertexArrayBindingDivisor(_vao.id(), 1, 1);

    glVertexArrayElementBuffer(_vao.id(), quad_EBO._id);
}

SpriteBatch::SpriteBatch(SpriteBatch &&other) noexcept
{
    _vao = std::move(other._vao);
    instances = std::move(other.instances);
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
    _current_buffer_index = other._current_buffer_index;
}

SpriteBatch &SpriteBatch::operator=(SpriteBatch &&other) noexcept
{
    if(this != &other)
    {
        _vao = std::move(other._vao);
        instances = std::move(other.instances);
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
        _current_buffer_index = other._current_buffer_index;
    }
    return *this;
}

SpriteBatch::~SpriteBatch()
{
    instances.clear();
}


void SpriteBatch::begin_batch()
{
    if(instances.empty()) return;

    _current_buffer_index++;
    _current_buffer_index %= BUFFER_COUNT;

#ifndef FLAG_RELEASE
    if(instances.size() > _texture->max_instances())
    {
        util::mtrs_warning("The number of instances is ", instances.size(),
            ", which has exceeded the MAX_INSTANCES limit of ", _texture->max_instances(),
            " in the SpriteBatch resource.");
    }
#endif

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
}

void SpriteBatch::draw_instances(const std::vector<InstanceData> &instances)
{
    if(this->instances.empty()) return;

    std::memcpy(_mapped_buffers[_current_buffer_index], instances.data(),
        instances.size() * sizeof(InstanceData));
    
    _vao.bind();
    _vao.set_vertex_buffer(1, _instance_vbo[_current_buffer_index], sizeof(InstanceData), 0);
    
    _shader->use();
    _texture->bind();
    
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)instances.size());
}

void SpriteBatch::end_batch()
{
    if(instances.empty()) return;

    _fences[_current_buffer_index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    size_t count;
    for(auto iter = instances.begin(); iter != instances.end(); iter++)
    {
        count = iter->second.size();
        iter->second.clear();
        if(count)
        {
            iter->second.reserve(count);
        }
    }
}

void SpriteBatch::flush()
{

}

}
