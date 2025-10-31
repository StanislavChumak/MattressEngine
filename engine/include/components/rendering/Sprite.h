#ifndef SPRITE_H
#define SPRITE_H

#include "../../../renderer/Texture2D.h"
#include "../../../renderer/ShaderProgram.h"
#include "../../../renderer/VertexArrayObject.h"
#include "../../../renderer/BufferObject.h"
#include <string>

struct Sprite2D {
    Texture2D &texture;
    ShaderProgram &shader;
    Texture2D::SubTexture2D *subTextures;
    int CountSubTextures;

    float layer = 0.f;

    BufferObject VBO;
    BufferObject VBOtex;
    BufferObject EBO;
    VertexArrayObject VAO;

    bool visibility;
};

#endif
