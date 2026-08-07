#ifndef LABEL_HPP
#define LABEL_HPP

#include "comp/Component.hpp"

#include "res/text/Text.hpp"

#include <memory>

namespace mtrs::res
{
    class ShaderProgram;
    class Font;
    class Texture;
}

namespace mtrs::comp
{

struct Label : public Component<Label>
{
    res::Text text;

    std::shared_ptr<res::ShaderProgram> shader;
    
    glm::uvec2 size;
    glm::uvec2 symbol_size;
    glm::tvec4<uint8_t, glm::precision::highp> color;
    float layer;

    bool visibility =  true;

    Label() = delete;
    Label(COMPONENT_ARGS);

    static constexpr const char *get_type_name_imp() noexcept { return "Label"; }
};

}

#endif
