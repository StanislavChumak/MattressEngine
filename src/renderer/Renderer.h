#ifndef RENDERER_H
#define RENDERER_H

#include <glm/fwd.hpp>

namespace RenderEngine
{
    class ShaderProgram;
    class VertexArrayObject;
    class BufferObject;

    class Renderer
    {
    public:
        static void draw(const ShaderProgram &shaderProgram, const VertexArrayObject &vertexArrayObject, const BufferObject &elementBuferObject);
        static void set_clear_color(const float r, const float g, const float b, const float a);
        static void set_depth_test(const bool enable);
        static void clear();
        static void set_viewport(const unsigned int width, const unsigned int height, const unsigned int offsetX = 0, const unsigned int offsetY = 0);
        
        static char *get_renderer_string();
        static char *get_GL_version_string();
        static char *get_GLSL_version_string();
        static glm::uvec2 get_offset_viewport();
    private:
        static glm::uvec2 _offsetViewport;
    };
}

#endif
