#include "res/asset/ShaderProgram.hpp"

#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "util/files/data_mtrs_file.hpp"
#include "util/mtrs_message.hpp"

#include "dynamic_field.def"
#include "res_struct/Shader.struct"

#include <fstream>

namespace mtrs::res
{

std::string shader_path_to_string(std::string path)
{
    std::ifstream shader;
    std::string buffer;

    shader.open(path, std::ios::ate | std::ios::binary);
#ifndef FLAG_RELEASE
    if (!shader.is_open())
    {
        util::mtrs_error("Failed to open shader: ", path);
        return "";
    }
#endif
    buffer.resize(shader.tellg());
    shader.seekg(0);

    shader.read(buffer.data(), buffer.size());

    return buffer;
}

bool create_shader(const char *sourse, const uint32_t &shader_type, uint32_t &shader_id)
{
    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &sourse, nullptr);
    glCompileShader(shader_id);
#ifndef FLAG_RELEASE
    GLint success = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar info_log[1024];
        glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);
        util::mtrs_error("Shader (type: ", shader_type, ") compilation failed:\n",
            info_log);
        return false;
    }
#endif
    return true;
}

ShaderProgram::ShaderProgram(ASSET_ARGS)
{
    Shader_rs shader;
    file.read(reinterpret_cast<char*>(&shader), sizeof(shader));

    std::string str_buffer;

    file::set_string_from_mtrs_file(file, str_buffer, DYNAMIC_ARGS(shader, vertex));
    str_buffer = shader_path_to_string(dir_resource + std::move(str_buffer));
#ifndef FLAG_RELEASE
    if (str_buffer.empty())
    {
        util::mtrs_error("Vertex shader no code");
        return;
    }
#endif
    GLuint vertex_id = 0;
    if(!create_shader(std::move(str_buffer).c_str(), GL_VERTEX_SHADER, vertex_id))
    {
#ifndef FLAG_RELEASE
        util::mtrs_error("Vertex shader compilation failed");
        return;
#endif
    }
    
    file::set_string_from_mtrs_file(file, str_buffer, DYNAMIC_ARGS(shader, fragment));
    str_buffer = shader_path_to_string(dir_resource + std::move(str_buffer));
#ifndef FLAG_RELEASE
    if (str_buffer.empty())
    {
        util::mtrs_error("Fragment shader no code");
        return;
    }
#endif
    GLuint fragment_id = 0;
    if(!create_shader(std::move(str_buffer).c_str(), GL_FRAGMENT_SHADER, fragment_id))
    {
#ifndef FLAG_RELEASE
        util::mtrs_error("Fragment shader compilation failed");
        glDeleteShader(vertex_id);
        return;
#endif
    }

    _ID = glCreateProgram();
    glAttachShader(_ID, vertex_id);
    glAttachShader(_ID, fragment_id);
    glLinkProgram(_ID);
#ifndef FLAG_RELEASE
    GLint success = 0;
    glGetProgramiv(_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar info_log[1024];
        glGetProgramInfoLog(_ID, 1024, nullptr, info_log);
        util::mtrs_error("Program linking failed:\n", info_log);
        _is_compiled = false;
    }
    else
#endif
    {
        _is_compiled = true;
    }
    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
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