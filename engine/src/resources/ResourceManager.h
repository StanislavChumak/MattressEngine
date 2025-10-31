#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <glm/fwd.hpp>

#include "renderer/ShaderProgram.h"
#include "renderer/Texture2D.h"
#include "sound/Sound.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class ShaderProgram;
class Texture2D;
class Sound;

typedef std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> ShaderProgramsMap;
typedef std::unordered_map<std::string, std::unique_ptr<Texture2D>> Textures2DMap;
typedef std::unordered_map<std::string, std::unique_ptr<Sound>> SoundsMap;

class ResourceManager
{
private:
    ShaderProgramsMap _shaderProgramsMap;
    Textures2DMap _textures2DMap;
    SoundsMap _soundsMap;

    std::string get_file_string(const std::string &relativeFilePath);
    std::string _executablePath;
    
public:
    ResourceManager(const std::string &executablePath);
    ~ResourceManager();
    void clear();

    ShaderProgram *load_shaders(const std::string &shaderName, const std::string &vertexPath, const std::string &fragmentPath);
    ShaderProgram *get_shader_program(const std::string &shaderName);

    Texture2D *load_texture2D(const std::string &textureName, const std::string &texturePath, const unsigned char &textureNumber = 0);
    Texture2D *get_texture2D(const std::string &textureName);
    Texture2D *load_texture_atlas(const std::string &textureName, const std::string &texturePath, const std::vector<std::string> &subTextureNames,
                                              const unsigned int &subTextureWidth, const unsigned int &subTextureHeight, const unsigned char &textureNumber = 0);

    Sound *load_sound(const std::string &soundName, const std::string &soundPath, const unsigned int &soundFlag);
    Sound *get_sound(const std::string &soundName);

    bool load_JSON_resources(const std::string &JSONPath);
};

#endif
