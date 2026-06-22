#include "res/render/VertexArrayObject.hpp"

#include "res/render/BufferObject.hpp"

namespace mtrs::res
{

VertexArrayObject::VertexArrayObject()
{
    glCreateVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject() noexcept
{
    if (_id != 0) glDeleteVertexArrays(1, &_id);
    _id = 0;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject &&other) noexcept
{
    _id = other._id;
    other._id = 0;
}

VertexArrayObject &VertexArrayObject::operator=(VertexArrayObject &&other) noexcept
{
    if(this != &other)
    {
        if (_id != 0) glDeleteVertexArrays(1, &_id);

        _id = other._id;
        other._id = 0;
    }
    return *this;
}

void VertexArrayObject::add_attrib_float(GLuint attrib_index, GLuint binding_index,
    GLint size, GLintptr rel_offset)
{
    glEnableVertexArrayAttrib(_id, attrib_index);
    glVertexArrayAttribBinding(_id, attrib_index, binding_index);
    glVertexArrayAttribFormat(_id, attrib_index, size, GL_FLOAT, GL_FALSE, rel_offset);
}

void VertexArrayObject::add_attrib_byteN(GLuint attrib_index, GLuint binding_index,
    GLint size, GLintptr rel_offset)
{
    glEnableVertexArrayAttrib(_id, attrib_index);
    glVertexArrayAttribBinding(_id, attrib_index, binding_index);
    glVertexArrayAttribFormat(_id, attrib_index, size, GL_UNSIGNED_BYTE, GL_TRUE, rel_offset);
}

void VertexArrayObject::set_vertex_buffer(GLuint binding_index, const BufferObject &buffer,
    GLsizei stride, GLintptr offset)
{
    glVertexArrayVertexBuffer(_id, binding_index, buffer.id(), offset, stride);
}

void VertexArrayObject::bind() const
{
    glBindVertexArray(_id);
}

void VertexArrayObject::unbind() const
{
    glBindVertexArray(0);
}

}