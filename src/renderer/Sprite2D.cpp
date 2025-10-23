#include "Sprite2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Renderer.h"

#include "../system/String.h"

namespace RenderEngine
{
    Sprite2D::Sprite2D(ShaderProgram *const shaderProgram, Texture2D *const texture,
                       const sys::Str &nameSubTexture, const sys::Str &nameUsage, const glm::vec3 &color)
        : _shaderProgram(std::move(shaderProgram)), _texture(std::move(texture)), _lastFrameId(0), _visibility(true)
    {
        auto subTexture = texture->get_sub_texture(nameSubTexture.c_str());

        const GLfloat vertexData[] = {
            //vertex cord   //color
            1.0f, 1.0f,     color.x, color.y, color.z, // right top
            1.0f, 0.0f,     color.x, color.y, color.z, // right bottom
            0.0f, 1.0f,     color.x, color.y, color.z, // left top
            0.0f, 0.0f,     color.x, color.y, color.z  // left bottom
        };
        const GLfloat textureData[] = {
            //texture
            subTexture.rightTopVertex.x, subTexture.rightTopVertex.y,
            subTexture.rightTopVertex.x, subTexture.leftBottomVertex.y,
            subTexture.leftBottomVertex.x, subTexture.rightTopVertex.y,
            subTexture.leftBottomVertex.x, subTexture.leftBottomVertex.y
        };
        const GLuint indices[] = {
            0, 1, 2, // first Triangle
            1, 2, 3  // second Triangle
        };

        GLenum usage;
        if(nameUsage == "stream") usage = GL_STREAM_DRAW;
        else if(nameUsage == "dynamic") usage = GL_DYNAMIC_DRAW;
        else if(nameUsage == "static") usage = GL_STATIC_DRAW;

        _VAO.bind();

        _VBO.init(GL_ARRAY_BUFFER, vertexData, sizeof(vertexData), usage);
        _VBOtex.init(GL_ARRAY_BUFFER, textureData, sizeof(textureData), usage);

        _VAO.add_buffer_float(_VBO, 2, 5, 0);

        _VAO.add_buffer_float(_VBOtex, 2, 2, 0);

        _VAO.add_buffer_float(_VBO, 3, 5, 2);

        _EBO.init(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), usage);
    }

    Sprite2D::~Sprite2D()
    {
    }

    void Sprite2D::render(const glm::vec2 &position, const glm::vec2 &size, const float &rotation, const float &layer, const size_t &frameId) const
    {
        if(!_visibility)
        {
            return;
        }

        if (_lastFrameId != frameId)
        {
            _lastFrameId = frameId;

            const FrameDescription &currentFrameDescription = _framesDescriptions[frameId];

            const GLfloat textureCoords[] = {
            currentFrameDescription.rightTopVertex.x, currentFrameDescription.rightTopVertex.y,    // right top
            currentFrameDescription.rightTopVertex.x, currentFrameDescription.leftBottomVertex.y,  // right bottom
            currentFrameDescription.leftBottomVertex.x, currentFrameDescription.rightTopVertex.y,  // left top
            currentFrameDescription.leftBottomVertex.x, currentFrameDescription.leftBottomVertex.y // left bottom
            };
            _VBOtex.update(textureCoords, sizeof(textureCoords), 2 * sizeof(GLfloat));
        }

        _shaderProgram->use();

        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(0.5f * size.x + position.x, 0.5f * size.y + position.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));

        _VAO.bind();
        _shaderProgram->set_matrix4("modelMatrix", model);
        _shaderProgram->set_float("layer", layer);

        _texture->active();
        _texture->bind();

        Renderer::draw(*_shaderProgram, _VAO, _EBO);
    }

    void Sprite2D::insert_frames(const std::vector<FrameDescription> &framesDescriptions)
    {
        _framesDescriptions = framesDescriptions;
    }

    double Sprite2D::get_frame_duration(const size_t frameId) const
    {
        return _framesDescriptions[frameId].duration;
    }

    size_t Sprite2D::get_frames_count() const
    {
        return _framesDescriptions.size();
    }
}
