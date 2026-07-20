#include "res/asset/RenderContext.hpp"

#include "glad/glad.h"

#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"

#include <algorithm>

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

void RenderContext::create_sprite_batch(std::shared_ptr<const ShaderProgram> shader,
    std::shared_ptr<const Texture> texture)
{
    uint64_t id = shader->id() | (uint64_t(texture->id()) << 32);

    if (_batches.find(id) != _batches.end()) return;

    _batches.emplace(id, SpriteBatch(_quad_VBO, _quad_EBO, std::move(shader), std::move(texture)));
    _iters_batches.push_back(_batches.find(id));
}

void RenderContext::submit_batch(uint64_t id, float layer, InstanceData date)
{
    _batches.at(id).instances[layer].push_back(std::move(date));
}

void RenderContext::draw()
{
    _iters_instences.reserve(_iters_batches.size());
    for(auto &iter : _iters_batches)
    {
        if(!iter->second.instances.empty())
        {
            iter->second.begin_batch();
            _iters_instences.push_back({iter->second, iter->second.instances.begin()});
        }
    }

    float layer;
    while(!_iters_instences.empty())
    {
        layer = _iters_instences.begin()->second->first;
        for(auto iter = _iters_instences.begin(); iter < _iters_instences.end(); iter++)
        {
            if(iter->second->first < layer)
            {
                layer = iter->second->first;
            }
        }

        for(auto iter = _iters_instences.begin(); iter < _iters_instences.end();)
        {
            if(iter->second->first == layer)
            {
                iter->first.draw_instances(iter->second->second);
                iter->second++;
                if(iter->second == iter->first.instances.end())
                {
                    std::iter_swap(_iters_instences.end() - 1, iter);
                    _iters_instences.pop_back();
                }
                else iter++;
            }
            else iter++;
        }        
    }
    
    for(auto &iter : _iters_batches)
    {
        if(!iter->second.instances.empty())
        {
            iter->second.end_batch();
        }
    }
}

}