#include "comp/single/Camera.hpp"

#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

namespace mtrs::comp
{
    Camera::Camera(util::ReactiveLeaf<glm::uvec2, glm::uvec2, 2> &window_size,
        glm::uvec2 size_in_pixels)
    : size_in_pixels(std::move(size_in_pixels))
    , offset_viewport([](const glm::uvec2 *win_sz, const glm::uvec2 *sz_pxs)
        {
            double ratio = static_cast<double>(sz_pxs->x) / sz_pxs->y;
            if (static_cast<double>(win_sz->x) / win_sz->y > ratio)
            {
                uint32_t width = win_sz->y * ratio;
                uint32_t x = (win_sz->x - width) >> 1;
                glViewport(x, 0, width, win_sz->y);
                return glm::uvec2{x, 0};
            }
            else
            {
                uint32_t height = win_sz->x / ratio;
                uint32_t y = (win_sz->y - height) >> 1;
                glViewport(0, y, win_sz->x, height);
                return glm::uvec2{0, y};
            }
        }, &window_size, &this->size_in_pixels)
    {
        window_size.add_observer(&offset_viewport);
        this->size_in_pixels.add_observer(&offset_viewport);

        UBO.init(GL_UNIFORM_BUFFER, &matrices, sizeof(matrices), GL_DYNAMIC_DRAW);
        UBO.bind_base(0);

        ui_UBO.init(GL_UNIFORM_BUFFER, &ui_projection, sizeof(ui_projection), GL_DYNAMIC_DRAW);
        ui_UBO.bind_base(1);

        update_proj_matrix();
    }

    void Camera::update_UBO()
    {
        UBO.update(&matrices, sizeof(matrices), 0);
        ui_UBO.update(&ui_projection, sizeof(ui_projection), 0);
    }

    void Camera::update_proj_matrix()
    {
        const glm::uvec2 &sz_in_pxs = size_in_pixels.get();
        matrices.proj = glm::ortho(0.f, (float)sz_in_pxs.x,
            0.f, (float)sz_in_pxs.y, -100.f, 100.f);
        ui_projection = glm::ortho(0.f, (float)sz_in_pxs.x,
            (float)sz_in_pxs.y, 0.f, -100.f, 100.f);
        update_UBO();
    }

    void Camera::update_view_matrix(glm::mat4 view)
    {
        matrices.view = std::move(view);
        UBO.update(&matrices, sizeof(matrices), 0);
    }
}