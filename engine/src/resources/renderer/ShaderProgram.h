#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glm/fwd.hpp>


class ShaderProgram
{
public:
    ShaderProgram(const char *vertexShaderCode, const char *fragmentShaderCode);
    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ShaderProgram &operator=(ShaderProgram &&shaderProgram) noexcept;
    ShaderProgram(ShaderProgram &&shaderProgram) noexcept;
    ~ShaderProgram();

    bool is_compiled() const { return _isCompiled; };
    void use() const;
    void set_int(const char* name, const int &value) const;
    void set_float(const char *name, const float &value) const;
    void set_matrix4(const char* name,  const glm::mat4 &matrix) const;

private:
    bool create_shader(const char *sourse, const unsigned int &shaderType, unsigned int &shaderID);
    bool _isCompiled = false;
    unsigned int _ID = 0;
};


#endif