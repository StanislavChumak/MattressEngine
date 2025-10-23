#include "ShaderProgram.h"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef FLAG_RELEASE
#include <iostream>
#endif

namespace RenderEngine
{
    ShaderProgram::ShaderProgram(const char *vertexShaderCode, const char *fragmentShaderCode)
    {
        GLuint vertexShaderID = 0;
        GLuint fragmentShaderID = 0;
        if (!create_shader(std::move(vertexShaderCode), GL_VERTEX_SHADER, vertexShaderID))
        {
#ifndef FLAG_RELEASE
            std::cerr << "ERROR::VERTEX::SHADER::COMPILATION_FAILED" << std::endl;
            return;
#endif
        }

        if (!create_shader(std::move(fragmentShaderCode), GL_FRAGMENT_SHADER, fragmentShaderID))
        {
#ifndef FLAG_RELEASE
            std::cerr << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
#endif
        }

        _ID = glCreateProgram();
        glAttachShader(_ID, vertexShaderID);
        glAttachShader(_ID, fragmentShaderID);
        glLinkProgram(_ID);
#ifndef FLAG_RELEASE
        GLint success = 0;
        glGetProgramiv(_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(_ID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED: \n"
                      << infoLog << std::endl;
            _isCompiled = false;
        }
        else
#endif
        {
            _isCompiled = true;
        }
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }

    bool ShaderProgram::create_shader(const char *sourse, const unsigned int &shaderType, unsigned int &shaderID)
    {
        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &sourse, nullptr);
        glCompileShader(shaderID);
#ifndef FLAG_RELEASE
        GLint success = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED (shader type " << shaderType << "):\n"
                      << infoLog << std::endl;
            return false;
        }
#endif
        return true;
    }

    ShaderProgram &ShaderProgram::operator=(ShaderProgram &&shaderProgram) noexcept
    {
        glDeleteProgram(_ID);
        _ID = shaderProgram._ID;
        _isCompiled = shaderProgram._isCompiled;

        shaderProgram._ID = 0;
        shaderProgram._isCompiled = false;
        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram) noexcept
    {
        _ID = shaderProgram._ID;
        _isCompiled = shaderProgram._isCompiled;

        shaderProgram._ID = 0;
        shaderProgram._isCompiled = false;
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(_ID);
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
}