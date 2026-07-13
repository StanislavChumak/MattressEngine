#include "res/asset/RenderContext.hpp"

#include "glad/glad.h"

#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

namespace mtrs::res
{

RenderContext::RenderContext(ASSET_ARGS)
{
    const float quad[] = {
        -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f
    };
    _quad_VBO.init(GL_ARRAY_BUFFER, quad, sizeof(quad), GL_STATIC_DRAW);
    
    const GLuint indices[] = {0, 1, 2, 2, 3, 0};
    _quad_EBO.init(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), GL_STATIC_DRAW);
}

RenderContext::RenderContext(RenderContext &&other) noexcept
{
    _quad_EBO = std::move(other._quad_EBO);
    _quad_VBO = std::move(other._quad_VBO);
    _batches = std::move(other._batches);
}

RenderContext &RenderContext::operator=(RenderContext &&other) noexcept
{
    if(this != &other)
    {
        _quad_EBO = std::move(other._quad_EBO);
        _quad_VBO = std::move(other._quad_VBO);
        _batches = std::move(other._batches);
    }
    return *this;
}

std::string RenderContext::get_type_name_imp() noexcept
{
    return "render_context";
}

uint32_t RenderContext::get_type_size_imp() noexcept
{
    return 0;
}

void RenderContext::create_sprite_batch(std::shared_ptr<const ShaderProgram> shader, std::shared_ptr<const Texture> texture)
{
    uint64_t id = shader->id() | uint64_t(texture->id()) << 32;

    if (_batches.find(id) != _batches.end()) return;

    SpriteBatch &batch = _batches.emplace(id, SpriteBatch()).first->second;
    _keys.push_back(id);

    batch._shader = std::move(shader);
    batch._texture = std::move(texture);

    for(uint8_t i = 0; i < SpriteBatch::BUFFER_COUNT; i++)
    {
        const uint64_t max_instances = batch._texture->max_instances();
        batch._instance_vbo[i]._mode = GL_ARRAY_BUFFER;
        glBindBuffer(GL_ARRAY_BUFFER, batch._instance_vbo[i]._id);
        glBufferStorage(GL_ARRAY_BUFFER, max_instances * sizeof(InstanceData), nullptr,
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

        batch._mapped_buffers[i] = reinterpret_cast<InstanceData*>(glMapBufferRange(
            GL_ARRAY_BUFFER, 0, max_instances * sizeof(InstanceData),
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));
    }

    batch._vao.set_vertex_buffer(0, _quad_VBO, sizeof(float) * 4, 0);
    batch._vao.add_attrib_float(0, 0, 2, 0);
    batch._vao.add_attrib_float(1, 0, 2, sizeof(float)*2);

    batch._vao.set_vertex_buffer(1, batch._instance_vbo[0], sizeof(InstanceData), 0);
    batch._vao.add_attrib_float (2, 1, 2, offsetof(InstanceData, position));
    batch._vao.add_attrib_float (3, 1, 2, offsetof(InstanceData, size));
    batch._vao.add_attrib_float (4, 1, 1, offsetof(InstanceData, rotation));
    batch._vao.add_attrib_float (5, 1, 2, offsetof(InstanceData, lb_uv));
    batch._vao.add_attrib_float (6, 1, 2, offsetof(InstanceData, rt_uv));
    batch._vao.add_attrib_byteN (7, 1, 4, offsetof(InstanceData, color));
    batch._vao.add_attrib_float (8, 1, 1, offsetof(InstanceData, layer));
    glVertexArrayBindingDivisor(batch._vao.id(), 1, 1);

    glVertexArrayElementBuffer(batch._vao.id(), _quad_EBO.id());
}

void RenderContext::begin_batches()
{
    for(auto &key : _keys)
    {
        _batches.at(key).begin_batch();
    }
}

void RenderContext::submit_batch(uint64_t id, InstanceData date)
{
    _batches.at(id).submit(std::move(date));
}

void RenderContext::end_batches()
{
    for(auto &key : _keys)
    {
        _batches.at(key).end_batch();
    }
}

}