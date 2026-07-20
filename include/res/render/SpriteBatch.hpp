#ifndef SPRITE_BATCH_HPP
#define SPRITE_BATCH_HPP

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "BufferObject.hpp"
#include "VertexArrayObject.hpp"

#include <map>
#include <vector>
#include <memory>

namespace mtrs::res
{

class ShaderProgram;
class Texture;

class RenderContext;

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
    
    std::shared_ptr<const ShaderProgram> _shader;
    std::shared_ptr<const Texture> _texture;
    
public:
    std::map<float, std::vector<InstanceData>> instances;

    SpriteBatch() = delete;
    SpriteBatch(const BufferObject &quad_VBO, const BufferObject &quad_EBO,
        std::shared_ptr<const ShaderProgram> shader, std::shared_ptr<const Texture> texture);
    SpriteBatch(const SpriteBatch&) = delete;
    SpriteBatch &operator=(const SpriteBatch&) = delete;
    SpriteBatch(SpriteBatch &&other) noexcept;
    SpriteBatch &operator=(SpriteBatch &&other) noexcept;
    ~SpriteBatch() noexcept;

    void begin_batch();
    void draw_instances(const std::vector<InstanceData> &instances);
    void end_batch();
    void flush();
};

}

#endif
