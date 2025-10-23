#include "Renderer.h"

#include "glm/vec2.hpp"

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "BufferObject.h"

namespace RenderEngine
{
    glm::uvec2 Renderer::_offsetViewport;

    void Renderer::draw(const ShaderProgram &shaderProgram, const VertexArrayObject &vertexArrayObject, const BufferObject &elementBuferObject)
    {
        shaderProgram.use();
        vertexArrayObject.bind();
        elementBuferObject.bind();

        glDrawElements(GL_TRIANGLES, elementBuferObject.get_count(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::set_clear_color(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void Renderer::set_depth_test(const bool enable)
    {
        if(enable) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::set_viewport(const unsigned int width, const unsigned int height, const unsigned int offsetX, const unsigned int offsetY)
    {
        glViewport(offsetX, offsetY, width, height);
        _offsetViewport.x = offsetX;
        _offsetViewport.y = offsetY;
    }

    glm::uvec2 Renderer::get_offset_viewport()
    {
        return _offsetViewport;
    }

    char *Renderer::get_renderer_string()
    {
        return (char *)glGetString(GL_RENDERER);
    }

    char *Renderer::get_GL_version_string()
    {
        return (char *)glGetString(GL_VERSION);
    }

    char *Renderer::get_GLSL_version_string()
    {
        return (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    }
}