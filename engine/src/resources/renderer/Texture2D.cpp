#include "Texture2D.h"

#include "glad/glad.h"

Texture2D::Texture2D(const unsigned int &width, const unsigned int &height,
                        const unsigned char *data, const int &channels,
                        const unsigned char &number,
                        const unsigned int &filter, const unsigned int &wrapMode)
                    : _width(width), _height(height), _number(number), _ID(0)
{
    switch (channels)
    {
    case 4:
        _mode = GL_RGBA;
        break;
    case 3:
        _mode = GL_RGB;
    }
    glGenTextures(1, &_ID);
    glActiveTexture(GL_TEXTURE0 + _number);
    glBindTexture(GL_TEXTURE_2D, _ID);
    glTexImage2D(GL_TEXTURE_2D, 0, _mode, _width, _height, 0, _mode, GL_UNSIGNED_BYTE, std::move(data));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D &Texture2D::operator=(Texture2D &&texture2D)
{
    glDeleteTextures(1, &_ID);
    _ID = texture2D._ID;
    texture2D._ID = 0;
    _mode = texture2D._mode;
    _width = texture2D._width;
    _height = texture2D._height;
    _number = texture2D._number;
    return *this;
}

Texture2D::Texture2D(Texture2D &&texture2D)
{
    _ID = texture2D._ID;
    texture2D._ID = 0;
    _mode = texture2D._mode;
    _width = texture2D._width;
    _height = texture2D._height;
    _number = texture2D._number;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &_ID);
}

void Texture2D::add_sub_texture(const std::string &subTextureName, const glm::vec2 &leftBottomVertex, const glm::vec2 &rightTopVertex)
{
    _subTexturesMap.emplace(subTextureName, SubTexture2D{leftBottomVertex, rightTopVertex});
}

const Texture2D::SubTexture2D &Texture2D::get_sub_texture(const std::string &subTextureName) const
{
    auto iterator = _subTexturesMap.find(subTextureName);
    if (iterator != _subTexturesMap.end())
    {
        return iterator->second;
    }
    const static SubTexture2D defaultSubTexture;
    return defaultSubTexture;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _ID);
}

void Texture2D::active() const
{
    glActiveTexture(GL_TEXTURE0 + _number);
}
