#include "res/asset/Texture.hpp"

#include "glad/glad.h"

#define  STB_IMAGE_IMPLEMENTATION
#define  STBI_ONLY_PNG
#include "stb_image.h"

#include "util/set_from_file_mtrs.hpp"
#include "util/mtrs_message.hpp"

#include "dynamic_field.def"
#include "res_struct/Texture.struct"

#include <fstream>

namespace mtrs::res
{

Texture::Texture(std::ifstream &file)
{
    Texture_rs texture;
    file.read(reinterpret_cast<char*>(&texture), sizeof(texture));
    
    std::string path;
    util::set_string_from_mtrs_file(file, path, DYNAMIC_ARGS(texture, path));

    _number = 0;
    // auto resultInt = obj["number"].get_int64();
    // if(!resultInt.error())
    //     _number = resultInt.value();

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    unsigned char *pixels = stbi_load(path.c_str(), &_width, &_height, &channels, 0);

    if (!pixels)
    {
        util::mtrs_message(util::TipeMessage::ERROR, "Failed to load texture: ", path);
        return;
    }
    
    switch (channels)
    {
    case 4:
        _mode = GL_RGBA;break;
    case 3:
        _mode = GL_RGB;
    }
    glGenTextures(1, &_ID);
    glActiveTexture(GL_TEXTURE0 + _number);
    glBindTexture(GL_TEXTURE_2D, _ID);
    glTexImage2D(GL_TEXTURE_2D, 0, _mode, _width, _height, 0, _mode, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture &&other) noexcept
{
    _ID = other._ID;
    other._ID = 0;
    _mode = other._mode;
    _width = other._width;
    _height = other._height;
    _number = other._number;
}

Texture &Texture::operator=(Texture &&other) noexcept
{
    if(this != &other)
    {
        glDeleteTextures(1, &_ID);
        _ID = other._ID;
        other._ID = 0;
        _mode = other._mode;
        _width = other._width;
        _height = other._height;
        _number = other._number;
    }
    return *this;
}

Texture::~Texture()
{
    glDeleteTextures(1, &_ID);
}

std::string Texture::get_type_name_imp() noexcept
{
    return "textures";
}

uint32_t Texture::get_type_size_imp() noexcept
{
    return sizeof(Texture_rs);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _ID);
}

void Texture::active() const
{
    glActiveTexture(GL_TEXTURE0 + _number);
}

uint32_t Texture::id() const noexcept
{
    return _ID;
}

}
