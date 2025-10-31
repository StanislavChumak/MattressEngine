#ifndef LRENDERER_H
#define LRENDERER_H


class lastRenderer
{
public:
    static void set_clear_color(const float r, const float g, const float b, const float a);
    static void set_depth_test(const bool enable);
    static void clear();
    
    static char *get_renderer_string();
    static char *get_GL_version_string();
    static char *get_GLSL_version_string();

};


#endif
