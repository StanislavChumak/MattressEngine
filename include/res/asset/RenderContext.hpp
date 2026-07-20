#ifndef RENDER_CONTEXT_HPP
#define RENDER_CONTEXT_HPP

#include "Asset.hpp"

#include "res/render/BufferObject.hpp"
#include "res/render/SpriteBatch.hpp"

#include <unordered_map>

namespace mtrs::res
{

class ShaderProgram;
class Texture;

class RenderContext : public Asset<RenderContext>
{
    BufferObject _quad_VBO;
    BufferObject _quad_EBO;

    std::unordered_map<uint64_t, SpriteBatch> _batches;
    std::vector<decltype(_batches)::iterator> _iters_batches;
    std::vector<std::pair<SpriteBatch&, decltype(SpriteBatch::instances)::iterator>> _iters_instences;
    
public:
    RenderContext(ASSET_ARGS);
    RenderContext(const RenderContext&) = delete;
    RenderContext &operator=(const RenderContext&) = delete;
    RenderContext(RenderContext &&other) noexcept;
    RenderContext &operator=(RenderContext &&other) noexcept;
    ~RenderContext() noexcept = default;

    static std::string get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    void create_sprite_batch(std::shared_ptr<const ShaderProgram> shader,
        std::shared_ptr<const Texture> texture);
    void submit_batch(uint64_t id, float layer, InstanceData date);
    void draw();
};

}

#endif
