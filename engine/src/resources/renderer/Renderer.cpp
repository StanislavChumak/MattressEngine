#include "Renderer.h"

#include "glm/vec2.hpp"

#include "VertexArrayObject.h"
#include "BufferObject.h"

void lastRenderer::set_clear_color(const float r, const float g, const float b, const float a)
{
    glClearColor(r, g, b, a);
}

void lastRenderer::set_depth_test(const bool enable)
{
    if(enable) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void lastRenderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

char *lastRenderer::get_renderer_string()
{
    return (char *)glGetString(GL_RENDERER);
}

char *lastRenderer::get_GL_version_string()
{
    return (char *)glGetString(GL_VERSION);
}

char *lastRenderer::get_GLSL_version_string()
{
    return (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
}
