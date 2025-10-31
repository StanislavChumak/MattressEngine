#ifndef SPRITE2D_H
#define SPRITE2D_H

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include "BufferObject.h"
#include "VertexArrayObject.h"

#include <vector>
#include <string>

namespace RenderEngine
{
    class ShaderProgram;
    class Texture2D;

    class Sprite2D
    {
    public:
        struct FrameDescription
        {
            FrameDescription(const glm::vec2 &leftBottom, const glm::vec2 &rightTop, const double &dur)
            : leftBottomVertex(leftBottom), rightTopVertex(rightTop), duration(dur)
            {
            }
            glm::vec2 leftBottomVertex;
            glm::vec2 rightTopVertex;
            double duration;
        };

        Sprite2D(ShaderProgram* const shaderProgram, Texture2D* const texture,
                 const std::string &nameSubTexture, const std::string &nameUsage, const glm::vec3 &color);
        Sprite2D(const Sprite2D &) = delete;
        Sprite2D &operator=(const Sprite2D &) = delete;
        Sprite2D &operator=(Sprite2D &&buffer) = delete;
        Sprite2D(Sprite2D &&buffer) = delete;
        ~Sprite2D();


        void render(const glm::vec2 &position, const glm::vec2 &size, const float &rotation, const float &layer = 0.0f, const size_t &frameId = 0) const;

        //------------

        void insert_frames(const std::vector<FrameDescription> &framesDescriptions);
        double get_frame_duration(const size_t frameId) const;
        size_t get_frames_count() const;

        void set_visibility(bool visibility) { _visibility = visibility; }
        bool get_visibility() { return _visibility; }

    protected:
        ShaderProgram* _shaderProgram;
        Texture2D* _texture;
        // BufferObject _VBO;
        // BufferObject _VBOtex;
        // BufferObject _EBO;
        // VertexArrayObject _VAO;

        mutable size_t _lastFrameId;

        bool _visibility;
    };
}

#endif
