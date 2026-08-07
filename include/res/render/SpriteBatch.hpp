#ifndef SPRITE_BATCH_HPP
#define SPRITE_BATCH_HPP

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "BufferObject.hpp"
#include "VertexArrayObject.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

namespace mtrs::res
{

struct InstanceData
{
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
    glm::vec2 lb_uv;
    glm::vec2 rt_uv;
    glm::tvec4<uint8_t, glm::precision::highp> color;
};

class SpriteBatch
{
    static constexpr uint8_t BUFFER_COUNT = 3;

    VertexArrayObject _vao;

    BufferObject _instance_vbo[BUFFER_COUNT];
    InstanceData* _mapped_buffers[BUFFER_COUNT] = {nullptr}; 
    GLsync _fences[BUFFER_COUNT] = {0};
    uint8_t _current_buffer_index = 0;
    
    uint32_t _shader;
    uint32_t _texture;
    uint64_t _max_instances;
    
public:
    std::unordered_map<float, std::vector<InstanceData>> instances;
    std::vector<float> layers;

    SpriteBatch() = delete;
    SpriteBatch(const BufferObject &quad_VBO, const BufferObject &quad_EBO,
        uint32_t shader, uint32_t texture, uint64_t max_instances);
    SpriteBatch(const SpriteBatch&) = delete;
    SpriteBatch &operator=(const SpriteBatch&) = delete;
    SpriteBatch(SpriteBatch &&other) noexcept;
    SpriteBatch &operator=(SpriteBatch &&other) noexcept;
    ~SpriteBatch() noexcept;

    void begin_batch();
    void draw_layer(float layer);
    void end_batch();
};

}

#endif
