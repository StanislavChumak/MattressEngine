#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <glm/fwd.hpp>

#include "res/asset/Asset.hpp"

#include <iosfwd>

namespace mtrs::res
{

class ShaderProgram : public Asset<ShaderProgram>
{
    bool create_shader(const char *sourse, const uint32_t &shader_type, uint32_t &shader_id);
    bool _is_compiled = false;
    uint32_t _ID = 0;

public:
    ShaderProgram(std::ifstream &file);
    ShaderProgram() = delete;
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram &operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram &&other) noexcept;
    ShaderProgram &operator=(ShaderProgram &&other) noexcept;
    ~ShaderProgram();

    static std::string get_type_name_imp() noexcept;
    static uint32_t get_type_size_imp() noexcept;

    bool is_compiled() const noexcept;
    void use() const;
    void set_int(const char* name, const int &value) const;
    void set_float(const char *name, const float &value) const;
    void set_matrix4(const char* name,  const glm::mat4 &matrix) const;
    bool has_uniform(const char *name) const;

    uint32_t id() const noexcept;    
};

}

#endif
