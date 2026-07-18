#include "comp/single/Camera.hpp"

#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

namespace mtrs::comp
{
    Camera::Camera(Window *window, glm::bvec2 fixed_sides, glm::uvec2 size_in_points)
    : size_in_points(std::move(size_in_points)), fixed_sides(fixed_sides)
    , viewport([](const glm::uvec2 *win_sz, const glm::uvec2 *sz_pnts)
        {
            double ratio = static_cast<double>(sz_pnts->x) / sz_pnts->y;
            if (static_cast<double>(win_sz->x) / win_sz->y > ratio)
            {
                uint32_t width = win_sz->y * ratio;
                uint32_t x = (win_sz->x - width) >> 1;
                glViewport(x, 0, width, win_sz->y);
                return glm::uvec4{x, 0, width, win_sz->y};
            }
            else
            {
                uint32_t height = win_sz->x / ratio;
                uint32_t y = (win_sz->y - height) >> 1;
                glViewport(0, y, win_sz->x, height);
                return glm::uvec4{0, y, win_sz->x, height};
            }
        }, &window->size, &this->size_in_points)
    , point_scale([](const glm::uvec2 *win_sz, const glm::uvec4 *view, const glm::uvec2 *sz_pnts)
        {
            return glm::vec2{view->z, view->w} / static_cast<glm::vec2>(*sz_pnts);
        }, &window->size, &viewport, &this->size_in_points)
    {
        window->size.add_observer(&viewport);
        this->size_in_points.add_observer(&viewport);
        window->size.add_observer(&point_scale);
        this->size_in_points.add_observer(&point_scale);

        if(!fixed_sides.x || !fixed_sides.y)
        {
            viewport.set_calculator([](const glm::uvec2 *win_sz, const glm::uvec2 *sz_pnts)
            {
                glViewport(0, 0, win_sz->x, win_sz->y);
                return glm::uvec4{0, 0, win_sz->x, win_sz->y};
            });
        }

        UBO.init(GL_UNIFORM_BUFFER, &matrices, sizeof(matrices), GL_DYNAMIC_DRAW);
        UBO.bind_base(0);

        ui_UBO.init(GL_UNIFORM_BUFFER, &ui_projection, sizeof(ui_projection), GL_DYNAMIC_DRAW);
        ui_UBO.bind_base(1);

        matrices.view = glm::mat4{1.f};

        update_proj_matrix();
    }

    void Camera::update_UBO()
    {
        UBO.update(&matrices, sizeof(matrices), 0);
        ui_UBO.update(&ui_projection, sizeof(ui_projection), 0);
    }

    void Camera::update_proj_matrix()
    {
        const glm::uvec2 &pnts_sz = size_in_points.get();
        matrices.proj = glm::ortho(pnts_sz.x * -0.5f, pnts_sz.x * 0.5f,
            pnts_sz.y * -0.5f, pnts_sz.y * 0.5f, -100.f, 100.f);
        ui_projection = glm::ortho(0.f, (float)pnts_sz.x,
            (float)pnts_sz.y, 0.f, -100.f, 100.f);
        update_UBO();
    }

    void Camera::update_view_matrix()
    {
        matrices.view = glm::inverse(*target_matrix);
        UBO.update(&matrices, sizeof(matrices), 0);
    }
}