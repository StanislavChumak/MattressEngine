#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <glm/fwd.hpp>

#include "res/asset/Asset.hpp"

namespace mtrs::res
{

class ShaderProgram
{
    bool createShader(const char *sourse, const uint32_t &shader_type, uint32_t &shader_id);
    bool _is_compiled = false;
    uint32_t _ID = 0;

public:
    ASSETS_CONSTRUCTORS(ShaderProgram);

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
