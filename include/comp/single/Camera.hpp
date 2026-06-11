#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "res/render/BufferObject.hpp"

#include "util/reactive/ReactiveValue.hpp"
#include "util/reactive/ReactiveLeaf.hpp"

namespace mtrs::comp
{

struct Camera
{
    util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> size_in_pixels;
    util::ReactiveValue<glm::uvec2, glm::uvec2, glm::uvec2> offset_viewport;
    
    res::BufferObject UBO;
    struct {
        glm::mat4 proj;
        glm::mat4 view;
    } matrices;

    Camera() = delete;
    ~Camera() = default;
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera&) = delete;
    Camera(Camera &&other) noexcept;
    Camera &operator=(Camera &&other) noexcept;

    Camera(util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> &window_size, glm::uvec2 size_in_pixels);
    
    void update_UBO();

    void update_proj_matrix();

    void update_view_matrix(glm::mat4 view);
};

}

#endif
