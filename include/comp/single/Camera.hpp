#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "comp/Component.hpp"

#ifndef SCRIPT
    #include "res/render/BufferObject.hpp"
#else
    namespace mtrs::res
    {
        typedef uint64_t BufferObject;
    }
#endif
#include "comp/single/Window.hpp"

#include "util/reactive/ReactiveStruct.hpp"

namespace mtrs::comp
{

struct Camera : public Component<Camera>
{
    react::ReactiveStruct<glm::uvec2, 3> size_in_points;
    react::ReactiveStruct<glm::uvec4, 1, glm::uvec2, glm::uvec2> viewport;
    react::ReactiveStruct<glm::vec2, 1, glm::uvec2, glm::uvec4, glm::uvec2> point_scale;
    
    res::BufferObject UBO, ui_UBO;
    struct {
        glm::mat4 proj;
        glm::mat4 view;
    } matrices;
    glm::mat4 ui_projection;

    const glm::mat4 *target_matrix = nullptr;

    const glm::bvec2 fixed_sides;

    Camera() = delete;
    ~Camera() = default;
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera &operator=(Camera&&) = delete;

    Camera(Window *window, glm::bvec2 fixed_sides, glm::uvec2 size_in_points);

    void update_UBO();

    void update_proj_matrix();

    void update_view_matrix();

    static constexpr const char *get_type_name_imp() noexcept { return "Camera"; }
};

}

#endif
