#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <glm/fwd.hpp>

#include <vector>
#include <map>

#include "../system/String.h"
#include "../system/Soloptr.h"

namespace RenderEngine
{
    class ShaderProgram;
    class Texture2D;
    class Sprite2D;
}
class Sound;

class ResourceManager
{
public:
    ResourceManager(const char *executablePath);
    ~ResourceManager();
    void clear();

    RenderEngine::ShaderProgram *load_shaders(const sys::Str &shaderName, const sys::Str &vertexPath, const sys::Str &fragmentPath);
    static RenderEngine::ShaderProgram *get_shader_program(const sys::Str &shaderName);

    RenderEngine::Texture2D *load_texture2D(const sys::Str &textureName, const sys::Str &texturePath, const unsigned char &textureNumber = 0);
    static RenderEngine::Texture2D *get_texture2D(const sys::Str &textureName);
    RenderEngine::Texture2D *load_texture_atlas(const sys::Str &textureName, const sys::Str &texturePath, const std::vector<sys::Str> &subTextureNames,
                                              const unsigned int &subTextureWidth, const unsigned int &subTextureHeight, const unsigned char &textureNumber = 0);

    RenderEngine::Sprite2D *load_sprite2D(const sys::Str &spriteName, const sys::Str &shaderName, const sys::Str &textureName,
                                         const sys::Str &subTextureName, const sys::Str nameUsage, const glm::vec3 &color);
    static RenderEngine::Sprite2D *get_sprite2D(const sys::Str &spriteName);

    Sound *load_sound(const sys::Str &soundName, const sys::Str &soundPath, const unsigned int &soundFlag);
    static Sound *get_sound(const sys::Str &soundName);

    bool load_JSON_resources(const sys::Str &JSONPath);

    typedef std::map<const sys::Str, sys::Soloptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
    typedef std::map<const sys::Str, sys::Soloptr<RenderEngine::Texture2D>> Textures2DMap;
    typedef std::map<const sys::Str, sys::Soloptr<RenderEngine::Sprite2D>> Sprites2DMap;
    typedef std::map<const sys::Str, sys::Soloptr<Sound>> SoundsMap;
private:
    sys::Str get_file_string(const sys::Str &relativeFilePath);

    static ShaderProgramsMap _shaderProgramsMap;
    static Textures2DMap _textures2DMap;
    static Sprites2DMap _sprites2DMap;
    static SoundsMap _soundsMap;

    sys::Str _executablePath;
};

#endif
