#ifndef VERTEX_ARRAY_OBJECT_HPP
#define VERTEX_ARRAY_OBJECT_HPP

#include "glad/glad.h"

namespace mtrs::res
{

class BufferObject;

class VertexArrayObject
{
    GLuint _id = 0;

public:
    VertexArrayObject();
    ~VertexArrayObject() noexcept;

    VertexArrayObject(const VertexArrayObject &) = delete;
    VertexArrayObject &operator=(const VertexArrayObject &) = delete;
    VertexArrayObject(VertexArrayObject &&other) noexcept;
    VertexArrayObject &operator=(VertexArrayObject &&other) noexcept;

    GLuint id() const noexcept { return _id; }

    void add_attrib_float(GLuint attrib_index, GLuint binding_index,
        GLint size, GLintptr rel_offset);
    void add_attrib_byteN(GLuint attrib_index, GLuint binding_index,
        GLint size, GLintptr rel_offset);
    void set_vertex_buffer(GLuint binding_index, const BufferObject &buffer,
        GLsizei stride, GLintptr offset);

    void bind() const;
    void unbind() const;
};


}

#endif
