#ifndef TEXTURE2D_H
#define TEXTURE2D_H


#include <glm/vec2.hpp>

#include <map>

namespace sys
{
    class Str;
}

namespace RenderEngine
{
    class Texture2D
    {
    public:
        struct SubTexture2D
        {
            glm::vec2 leftBottomVertex = glm::vec2(0.0f);
            glm::vec2 rightTopVertex = glm::vec2(1.0f);

            SubTexture2D(glm::vec2 leftBottom, glm::vec2 rightTop)
            : leftBottomVertex(leftBottom), rightTopVertex(rightTop)
            {}

            SubTexture2D(){}
        };

        Texture2D(const unsigned int &width, const unsigned int &height,
                  const unsigned char *data, const int &channels = 4,
                  const unsigned char &nuber = 0,
                  const unsigned int &filter = 0x2601, const unsigned int &wrapMode = 0x812F);
        Texture2D() = delete;
        Texture2D(Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        Texture2D &operator=(Texture2D &&Texture2D);
        Texture2D(Texture2D &&Texture2D);
        ~Texture2D();

        void add_sub_texture(const sys::Str &subTextureName, const glm::vec2 &leftBottomVertex, const glm::vec2 &rightTopVertex);
        const SubTexture2D& get_sub_texture(const sys::Str &subTextureName) const;
        unsigned int get_width() const { return _width;}
        unsigned int get_height() const { return _height;}
        void bind() const;
        void active() const;

    private:
        unsigned int _ID;
        unsigned char _number;
        unsigned int _mode;
        unsigned int _width;
        unsigned int _height;

        std::map<const sys::Str, SubTexture2D> _subTexturesMap;
    };
}

#endif
