#include "ResourceManager.h"

#include "glm/vec3.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../../external/stb_image.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "../renderer/Sprite2D.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Texture2D.h"
#include "../sound/Sound.h"


#include <fstream>

#ifndef FLAG_RELEASE
#include <iostream>
#endif


ResourceManager::ShaderProgramsMap ResourceManager::_shaderProgramsMap;
ResourceManager::Textures2DMap ResourceManager::_textures2DMap;
ResourceManager::Sprites2DMap ResourceManager::_sprites2DMap;
ResourceManager::SoundsMap ResourceManager::_soundsMap;

ResourceManager::ResourceManager(const char *executablePath)
{
    sys::Str exePath = executablePath;
    int i = exePath.size();
    while(executablePath[i] != '/' && executablePath[i] != '\\') i--;
    _executablePath = exePath.sub_str(i + 1);
}

ResourceManager::~ResourceManager()
{
    clear();
}

void ResourceManager::clear()
{
    _shaderProgramsMap.clear();
    _textures2DMap.clear();
    _sprites2DMap.clear();
}

sys::Str ResourceManager::get_file_string(const sys::Str &relativeFilePath)
{
    std::ifstream fileFlow;
    fileFlow.open(_executablePath + relativeFilePath, std::ios::ate | std::ios::binary);

#   ifndef FLAG_RELEASE
    if (!fileFlow.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath.c_str() << std::endl;
        return sys::Str{};
    }
#   endif

    std::streamsize size = fileFlow.tellg();
    fileFlow.seekg(0);

    char* buffer = new char[size + 1];
    fileFlow.read(buffer, size);
    buffer[size] = '\0';

    return sys::Str(buffer);
}

RenderEngine::ShaderProgram *ResourceManager::load_shaders(const sys::Str &shaderName, const sys::Str &vertexPath, const sys::Str &fragmentPath)
{
    sys::Str vertexShaderCode = get_file_string(vertexPath);
    sys::Str fragmentShaderCode = get_file_string(fragmentPath);

#   ifndef FLAG_RELEASE
    if (vertexShaderCode.empty())
    {
        std::cerr << "No vertex shader !" << std::endl;
        return nullptr;
    }
    
    if (fragmentShaderCode.empty())
    {
        std::cerr << "No fragment shader !" << std::endl;
        return nullptr;
    }
#   endif

    RenderEngine::ShaderProgram *newShader = _shaderProgramsMap.emplace(shaderName, sys::make_soloptr<RenderEngine::ShaderProgram>(vertexShaderCode.c_str(), fragmentShaderCode.c_str())).first->second.get();

#   ifndef FLAG_RELEASE
    if (!newShader->is_compiled())
    {
        std::cerr << "Can't load shader program: \n"
                << "Vertex: " << vertexPath.c_str() << "\n"
                << "Fragment: " << fragmentPath.c_str() << std::endl;
        return nullptr;
    }
#   endif

    return newShader;
}

RenderEngine::ShaderProgram *ResourceManager::get_shader_program(const sys::Str &shaderName)
{
    ShaderProgramsMap::const_iterator iter = _shaderProgramsMap.find(shaderName);

#   ifndef FLAG_RELEASE
    if (iter == _shaderProgramsMap.end())
    {
        std::cerr << "Can't find the shader program: " << shaderName.c_str() << std::endl;
        return nullptr;
    }
#   endif

    return iter->second.get();
}

RenderEngine::Texture2D *ResourceManager::load_texture2D(const sys::Str &textureName, const sys::Str &texturePath, const unsigned char &number)
{
    int channels = 0, width = 0, height = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load(_executablePath + texturePath, &width, &height, &channels, 0);

#   ifndef FLAG_RELEASE
    if (!pixels)
    {
        std::cerr << "Can't load image: " << texturePath.c_str() << std::endl;
        return nullptr;
    }
#   endif

    RenderEngine::Texture2D *newTexture2D = _textures2DMap.emplace(textureName, sys::make_soloptr<RenderEngine::Texture2D>(width, height, pixels, channels, number, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second.get();

    stbi_image_free(pixels);
    return newTexture2D;
}

RenderEngine::Texture2D *ResourceManager::get_texture2D(const sys::Str &textureName)
{
    Textures2DMap::const_iterator iter = _textures2DMap.find(textureName);

#   ifndef FLAG_RELEASE
    if (iter == _textures2DMap.end())
    {
        std::cerr << "Can't find the texture2D: " << textureName.c_str() << std::endl;
        return nullptr;
    }
#   endif
    return iter->second.get();
}

RenderEngine::Texture2D *ResourceManager::load_texture_atlas(const sys::Str &textureName, const sys::Str &texturePath,
                                                           const std::vector<sys::Str> &subTextureNames, const unsigned int &subTextureWidth,
                                                           const unsigned int &subTextureHeight, const unsigned char &textureNumber)
{
    auto texture = load_texture2D(std::move(textureName), std::move(texturePath), textureNumber);
    if (texture)
    {
        const unsigned int textureWidth = texture->get_width();
        const unsigned int textureHeight = texture->get_height();
        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;
        for (const auto &subTextureName : subTextureNames)
        {
            glm::vec2 leftBottom(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);
            glm::vec2 rigthTop(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);

            texture->add_sub_texture(subTextureName, leftBottom, rigthTop);

            currentTextureOffsetX += subTextureWidth;
            if (currentTextureOffsetX >= textureWidth)
            {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    }
    return texture;
}

RenderEngine::Sprite2D *ResourceManager::load_sprite2D(const sys::Str &spriteName, const sys::Str &shaderName, const sys::Str &textureName, const sys::Str &subTextureName, const sys::Str nameUsage, const glm::vec3 &color)
{
    auto shader = get_shader_program(shaderName);
    auto texture = get_texture2D(textureName);

#   ifndef FLAG_RELEASE
    if (!shader)
    {
        std::cerr << "Can't find the shaderProgram: " << shaderName.c_str() << " for the sprite: " << spriteName.c_str() << std::endl;
        return nullptr;
    }
    if (!texture)
    {
        std::cerr << "Can't find the texture2D: " << textureName.c_str() << " for the sprite: " << spriteName.c_str() << std::endl;
        return nullptr;
    }
#   endif

    RenderEngine::Sprite2D *newSprite2D = _sprites2DMap.emplace(spriteName, sys::make_soloptr<RenderEngine::Sprite2D>(shader, texture, subTextureName, nameUsage, color)).first->second.get();

    return newSprite2D;
}

RenderEngine::Sprite2D *ResourceManager::get_sprite2D(const sys::Str &spriteName)
{
    Sprites2DMap::const_iterator iter = _sprites2DMap.find(spriteName);

#   ifndef FLAG_RELEASE
    if (iter == _sprites2DMap.end())
    {
        std::cerr << "Can't find the sprite2D: " << spriteName.c_str() << std::endl;
        return nullptr;
    }
#   endif

    return iter->second.get();
}

Sound *ResourceManager::load_sound(const sys::Str &soundName, const sys::Str &soundPath, const unsigned int &soundFlag)
{
    Sound *newSound= _soundsMap.emplace(soundName, sys::make_soloptr<Sound>(_executablePath + soundPath, soundFlag)).first->second.get();
    return newSound;
}

Sound *ResourceManager::get_sound(const sys::Str &soundName)
{
    SoundsMap::const_iterator iter = _soundsMap.find(soundName);

#ifndef FLAG_RELEASE
    if (iter == _soundsMap.end())
    {
        std::cerr << "Can't find the Sound: " << soundName.c_str() << std::endl;
        return nullptr;
    }
#endif
    return iter->second.get();
}

bool ResourceManager::load_JSON_resources(const sys::Str &JSONPath)
{
    sys::Str JSONString = get_file_string(JSONPath);

#   ifndef FLAG_RELEASE
    if (JSONString.empty())
    {
        std::cerr << "No JSON resorces file!" << std::endl;
        return false;
    }
#   endif

    rapidjson::Document document;
    rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());

#   ifndef FLAG_RELEASE
    if (!parseResult)
    {
        std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << '(' << parseResult.Offset() << ')' << std::endl;
        std::cerr << "In JSON file:" << JSONPath.c_str() << std::endl;
        return false;
    }
#   endif

    auto shadersIterator = document.FindMember("shaders");
    if (shadersIterator != document.MemberEnd())
    {
        for (const auto &currentShader : shadersIterator->value.GetArray())
        {
            const sys::Str name = currentShader["name"].GetString();
            const sys::Str pathVertexFile = currentShader["pathVertexFile"].GetString();
            const sys::Str pathFragmentFile = currentShader["pathFragmentFile"].GetString();
            auto shaderSprite2D = load_shaders(name, pathVertexFile, pathFragmentFile);

#           ifndef FLAG_RELEASE
            if (!shaderSprite2D->is_compiled())
            {
                std::cerr << "Can't find Sheder Program: " << name.c_str() << std::endl;
                return false;
            }
#           endif
        }
    }

    auto textureAtlasesIterator = document.FindMember("textureAtlases");
    if (textureAtlasesIterator != document.MemberEnd())
    {
        for (const auto &currentTextureAtlases : textureAtlasesIterator->value.GetArray())
        {
            const sys::Str name = currentTextureAtlases["name"].GetString();
            const sys::Str filePath = currentTextureAtlases["filePath"].GetString();
            const unsigned int subTextureWidth = currentTextureAtlases["subTextureWidth"].GetUint();
            const unsigned int subTextureHeight = currentTextureAtlases["subTextureHeight"].GetUint();

            const auto subTexturesArray = currentTextureAtlases["subTextures"].GetArray();
            std::vector<sys::Str> subTextures;
            subTextures.reserve(subTexturesArray.Size());
            for (const auto &currentSubTexture : subTexturesArray)
            {
                subTextures.push_back(currentSubTexture.GetString());
            }
            auto textureAtlase = load_texture_atlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
#           ifndef FLAG_RELEASE
            if (!textureAtlase)
            {
                std::cerr << "Can't Create Texture Atlase: " << name.c_str() << std::endl;
                return false;
            }
#           endif
        }
    }

    auto spriteIterator = document.FindMember("sprite");
    if (spriteIterator != document.MemberEnd())
    {
        for (const auto &currentSpite : spriteIterator->value.GetArray())
        {
            const sys::Str nameSprite = currentSpite["name"].GetString();
            const sys::Str nameShader = currentSpite["shader"].GetString();
            const sys::Str nameTextureAtlas = currentSpite["textureAtlas"].GetString();
            const sys::Str nameSubTexture = currentSpite["initialSubTexture"].GetString();
            const sys::Str nameUsage = currentSpite["usage"].GetString();
            auto colorIterator = currentSpite.FindMember("color");
            RenderEngine::Sprite2D *sprite;
            if (colorIterator != currentSpite.MemberEnd())
            {
                const auto colorArray = colorIterator->value.GetArray();
                const glm::vec3 color = glm::vec3(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat());
                sprite = load_sprite2D(nameSprite, nameShader, nameTextureAtlas, nameSubTexture, nameUsage, color);
            }
            else
            {
                sprite = load_sprite2D(nameSprite, nameShader, nameTextureAtlas, nameSubTexture, nameUsage, glm::vec3(1.f));
            }

            if (!sprite)
            {
                continue;
            }

            auto framesIterator = currentSpite.FindMember("frames");
            if (framesIterator != currentSpite.MemberEnd())
            {
                const auto framesArray = framesIterator->value.GetArray();
                std::vector<RenderEngine::Sprite2D::FrameDescription> framesDescription;
                framesDescription.reserve(framesArray.Size());
                for (const auto &currentFrame : framesArray)
                {
                    const sys::Str nameSubTexture = currentFrame["subTexture"].GetString();
                    const double duration = currentFrame["duration"].GetDouble();
                    const auto textureAtlas = get_texture2D(nameTextureAtlas);
                    const auto subTexture = textureAtlas->get_sub_texture(nameSubTexture);
                    framesDescription.emplace_back(subTexture.leftBottomVertex, subTexture.rightTopVertex, duration);
                }
                sprite->insert_frames(std::move(framesDescription));
            }
        }
    }

    auto soundIterator = document.FindMember("sounds");
    if (soundIterator != document.MemberEnd())
    {
        for (const auto &currentSound : soundIterator->value.GetArray())
        {
            const sys::Str name = currentSound["name"].GetString();
            const sys::Str soundPath = currentSound["soundPath"].GetString();

            unsigned int flag = 0;

            auto currentFlag = currentSound["flag"].GetObject();

            if(currentFlag["NONE"].GetBool()) flag |= Sound::Flag::NONE;
            if(currentFlag["STREAM"].GetBool()) flag |= Sound::Flag::STREAM;
            if(currentFlag["LOOP"].GetBool()) flag |= Sound::Flag::LOOP;
            if(currentFlag["NO_SPATIALIZATION"].GetBool()) flag |= Sound::Flag::NO_SPATIALIZATION;

            auto sound = load_sound(name, soundPath, flag);
        }
    }

    return true;
}