#include "comp/ui/Label.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ShaderProgram.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/GlyphDecoder.hpp"

#include "util/func/files/data_mtrs_file.hpp"
#include "util/func/text/utf8_to_utf32.hpp"

#include <fstream>
#include <cstring>

#include "dynamic_field.def"
#include "comp_struct/Label.struct"

namespace mtrs::comp
{

Label::Label(COMPONENT_ARGS)
{
    Label_sc label;
    file.read(reinterpret_cast<char*>(&label), sizeof(label));
    
    std::string str;
    util::set_string_from_mtrs_file(file, str, DYNAMIC_ARGS(label, shader));
    shader = resource.get_resource<res::ShaderProgram>(scene, std::move(str));
    
    util::set_string_from_mtrs_file(file, str, DYNAMIC_ARGS(label, text));
    auto decoder = world.single_comp<GlyphDecoder>();
    text = decoder->decode_text(util::utf8_to_utf32(str));

    layer = label.layer;

    size.x = label.size_x;
    size.y = label.size_y;

    std::memcpy(&color, &label.color, 4);
}

}