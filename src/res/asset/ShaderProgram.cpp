#include "res/asset/ShaderProgram.hpp"

#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "util/get_from_file_mtrs.hpp"
#include "util/mtrs_message.hpp"

#include "mtrsstruct/dynamic_field.def"
#include "mtrsstruct/res_struct/Shader.struct"

#include <fstream>

namespace mtrs::res
{

ShaderProgram::ShaderProgram(std::ifstream &file)
{
    Shader_rs shader;
    file.read(reinterpret_cast<char*>(&shader), sizeof(shader));

    std::string vertex_shader_code, fragment_shader_code;
    vertex_shader_code = util::get_string_from_mtrs_file(file, DYNAMIC_ARGS(shader, vertex));
    fragment_shader_code = util::get_string_from_mtrs_file(file, DYNAMIC_ARGS(shader, fragment));

    if (vertex_shader_code.empty())
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Vertex shader no code");
        return;
    }
    
    if (fragment_shader_code.empty())
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Fragment shader no code");
        return;
    }

    GLuint vertex_id = 0;
    GLuint fragment_id = 0;
    if(!createShader(std::move(vertex_shader_code).c_str(), GL_VERTEX_SHADER, vertex_id))
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Vertex shader compilation failed");
        return;
    }

    if(!createShader(std::move(fragment_shader_code).c_str(), GL_FRAGMENT_SHADER, fragment_id))
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Fragment shader compilation failed");
        glDeleteShader(vertex_id);
        return;
    }

    _ID = glCreateProgram();
    glAttachShader(_ID, vertex_id);
    glAttachShader(_ID, fragment_id);
    glLinkProgram(_ID);
    GLint success = 0;
    glGetProgramiv(_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar info_log[1024];
        glGetProgramInfoLog(_ID, 1024, nullptr, info_log);
        util::mtrs_message(util::TipeMessage::ERROR, "Program linking failed:\n", info_log);
        _is_compiled = false;
    }
    else
    {
        _is_compiled = true;
    }
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
}

bool ShaderProgram::createShader(const char *sourse,const uint32_t &shader_type,uint32_t &shader_id)
{
    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &sourse, nullptr);
    glCompileShader(shader_id);
    GLint success = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar info_log[1024];
        glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);
        util::mtrs_message(util::TipeMessage::ERROR, "Shader (type: ", shader_type,
            ") compilation failed:\n", info_log);
        return false;
    }
    return true;
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept
{
    _ID = other._ID;
    _is_compiled = other._is_compiled;
    other._ID = 0;
    other._is_compiled = false;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept
{
    if(this != &other)
    {
        glDeleteProgram(_ID);
        _ID = other._ID;
        _is_compiled = other._is_compiled;
        other._ID = 0;
        other._is_compiled = false;
    }
    return *this;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_ID);
}

std::string ShaderProgram::get_type_name_imp() noexcept
{
    return "shaders";
}

uint32_t ShaderProgram::get_type_size_imp() noexcept
{
    return sizeof(Shader_rs);
}

bool ShaderProgram::is_compiled() const noexcept
{
    return _is_compiled;
}

void ShaderProgram::use() const
{
    glUseProgram(_ID);
}

void ShaderProgram::set_int(const char *name, const int &value) const
{
    glUniform1i(glGetUniformLocation(_ID, name), value);
}

void ShaderProgram::set_float(const char *name, const float &value) const
{
    glUniform1f(glGetUniformLocation(_ID, name), value);
}

void ShaderProgram::set_matrix4(const char *name, const glm::mat4 &matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(_ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

bool ShaderProgram::has_uniform(const char *name) const {
    return glGetUniformLocation(_ID, name) != -1;
}

uint32_t ShaderProgram::id() const noexcept
{
    return _ID;
}

}