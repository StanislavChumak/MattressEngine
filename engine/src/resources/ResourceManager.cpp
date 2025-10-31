#include "ResourceManager.h"

#include "glad/glad.h"

#include "glm/vec3.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../../external/stb_image.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include <fstream>

#ifndef FLAG_RELEASE
#include <iostream>
#endif

ResourceManager::ResourceManager(const std::string &executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    _executablePath = executablePath.substr(0, found + 1);
}

ResourceManager::~ResourceManager()
{
    clear();
}

void ResourceManager::clear()
{
    _shaderProgramsMap.clear();
    _textures2DMap.clear();
    _soundsMap.clear();
}

std::string ResourceManager::get_file_string(const std::string &relativeFilePath)
{
    std::ifstream fileFlow;
    fileFlow.open(_executablePath + relativeFilePath, std::ios::ate | std::ios::binary);

#   ifndef FLAG_RELEASE
    if (!fileFlow.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return "";
    }
#   endif

    std::streamsize size = fileFlow.tellg();
    fileFlow.seekg(0);

    char* buffer = new char[size + 1];
    fileFlow.read(buffer, size);
    buffer[size] = '\0';

    return std::string(buffer);
}

ShaderProgram *ResourceManager::load_shaders(const std::string &shaderName, const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexShaderCode = get_file_string(vertexPath);
    std::string fragmentShaderCode = get_file_string(fragmentPath);

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

    ShaderProgram *newShader = _shaderProgramsMap.emplace(shaderName, std::make_unique<ShaderProgram>(vertexShaderCode.c_str(), fragmentShaderCode.c_str())).first->second.get();

#   ifndef FLAG_RELEASE
    if (!newShader->is_compiled())
    {
        std::cerr << "Can't load shader program: \n"
                << "Vertex: " << vertexPath << "\n"
                << "Fragment: " << fragmentPath << std::endl;
        return nullptr;
    }
#   endif

    return newShader;
}

ShaderProgram *ResourceManager::get_shader_program(const std::string &shaderName)
{
    ShaderProgramsMap::const_iterator iter = _shaderProgramsMap.find(shaderName);

#   ifndef FLAG_RELEASE
    if (iter == _shaderProgramsMap.end())
    {
        std::cerr << "Can't find the shader program: " << shaderName << std::endl;
        return nullptr;
    }
#   endif

    return iter->second.get();
}

Texture2D *ResourceManager::load_texture2D(const std::string &textureName, const std::string &texturePath, const unsigned char &number)
{
    int channels = 0, width = 0, height = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load((_executablePath + texturePath).c_str(), &width, &height, &channels, 0);

#   ifndef FLAG_RELEASE
    if (!pixels)
    {
        std::cerr << "Can't load image: " << texturePath << std::endl;
        return nullptr;
    }
#   endif

   Texture2D *newTexture2D = _textures2DMap.emplace(textureName, std::make_unique<Texture2D>(width, height, pixels, channels, number, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second.get();

    stbi_image_free(pixels);
    return newTexture2D;
}

Texture2D *ResourceManager::get_texture2D(const std::string &textureName)
{
    Textures2DMap::const_iterator iter = _textures2DMap.find(textureName);

#   ifndef FLAG_RELEASE
    if (iter == _textures2DMap.end())
    {
        std::cerr << "Can't find the texture2D: " << textureName << std::endl;
        return nullptr;
    }
#   endif
    return iter->second.get();
}

Texture2D *ResourceManager::load_texture_atlas(const std::string &textureName, const std::string &texturePath,
                                                           const std::vector<std::string> &subTextureNames, const unsigned int &subTextureWidth,
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

// Sprite2D *ResourceManager::load_sprite2D(const std::string &spriteName, const std::string &shaderName, const std::string &textureName, const std::string &subTextureName, const std::string nameUsage, const glm::vec3 &color)
// {
//     auto shader = get_shader_program(shaderName);
//     auto texture = get_texture2D(textureName);

// #   ifndef FLAG_RELEASE
//     if (!shader)
//     {
//         std::cerr << "Can't find the shaderProgram: " << shaderName << " for the sprite: " << spriteName << std::endl;
//         return nullptr;
//     }
//     if (!texture)
//     {
//         std::cerr << "Can't find the texture2D: " << textureName << " for the sprite: " << spriteName << std::endl;
//         return nullptr;
//     }
// #   endif

//     RenderEngine::Sprite2D *newSprite2D = _sprites2DMap.emplace(spriteName, utl::make_soloptr<RenderEngine::Sprite2D>(shader, texture, subTextureName, nameUsage, color)).first->second.get();

//     return newSprite2D;
// }

// Sprite2D *ResourceManager::get_sprite2D(const std::string &spriteName)
// {
//     Sprites2DMap::const_iterator iter = _sprites2DMap.find(spriteName);

// #   ifndef FLAG_RELEASE
//     if (iter == _sprites2DMap.end())
//     {
//         std::cerr << "Can't find the sprite2D: " << spriteName << std::endl;
//         return nullptr;
//     }
// #   endif

//     return iter->second.get();
// }

Sound *ResourceManager::load_sound(const std::string &soundName, const std::string &soundPath, const unsigned int &soundFlag)
{
    Sound *newSound= _soundsMap.emplace(soundName, std::make_unique<Sound>((_executablePath + soundPath).c_str(), soundFlag)).first->second.get();
    return newSound;
}

Sound *ResourceManager::get_sound(const std::string &soundName)
{
    SoundsMap::const_iterator iter = _soundsMap.find(soundName);

#ifndef FLAG_RELEASE
    if (iter == _soundsMap.end())
    {
        std::cerr << "Can't find the Sound: " << soundName << std::endl;
        return nullptr;
    }
#endif
    return iter->second.get();
}

bool ResourceManager::load_JSON_resources(const std::string &JSONPath)
{
    std::string JSONString = get_file_string(JSONPath);

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
        std::cerr << "In JSON file:" << JSONPath << std::endl;
        return false;
    }
#   endif

    auto shadersIterator = document.FindMember("shaders");
    if (shadersIterator != document.MemberEnd())
    {
        for (const auto &currentShader : shadersIterator->value.GetArray())
        {
            const std::string name = currentShader["name"].GetString();
            const std::string pathVertexFile = currentShader["pathVertexFile"].GetString();
            const std::string pathFragmentFile = currentShader["pathFragmentFile"].GetString();
            auto shaderSprite2D = load_shaders(name, pathVertexFile, pathFragmentFile);

#           ifndef FLAG_RELEASE
            if (!shaderSprite2D->is_compiled())
            {
                std::cerr << "Can't find Sheder Program: " << name << std::endl;
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
            const std::string name = currentTextureAtlases["name"].GetString();
            const std::string filePath = currentTextureAtlases["filePath"].GetString();
            const unsigned int subTextureWidth = currentTextureAtlases["subTextureWidth"].GetUint();
            const unsigned int subTextureHeight = currentTextureAtlases["subTextureHeight"].GetUint();

            const auto subTexturesArray = currentTextureAtlases["subTextures"].GetArray();
            std::vector<std::string> subTextures;
            subTextures.reserve(subTexturesArray.Size());
            for (const auto &currentSubTexture : subTexturesArray)
            {
                subTextures.push_back(currentSubTexture.GetString());
            }
            auto textureAtlase = load_texture_atlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
#           ifndef FLAG_RELEASE
            if (!textureAtlase)
            {
                std::cerr << "Can't Create Texture Atlase: " << name << std::endl;
                return false;
            }
#           endif
        }
    }

    // auto spriteIterator = document.FindMember("sprite");
    // if (spriteIterator != document.MemberEnd())
    // {
    //     for (const auto &currentSpite : spriteIterator->value.GetArray())
    //     {
    //         const std::string nameSprite = currentSpite["name"].GetString();
    //         const std::string nameShader = currentSpite["shader"].GetString();
    //         const std::string nameTextureAtlas = currentSpite["textureAtlas"].GetString();
    //         const std::string nameSubTexture = currentSpite["initialSubTexture"].GetString();
    //         const std::string nameUsage = currentSpite["usage"].GetString();
    //         auto colorIterator = currentSpite.FindMember("color");
    //         RenderEngine::Sprite2D *sprite;
    //         if (colorIterator != currentSpite.MemberEnd())
    //         {
    //             const auto colorArray = colorIterator->value.GetArray();
    //             const glm::vec3 color = glm::vec3(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat());
    //             sprite = load_sprite2D(nameSprite, nameShader, nameTextureAtlas, nameSubTexture, nameUsage, color);
    //         }
    //         else
    //         {
    //             sprite = load_sprite2D(nameSprite, nameShader, nameTextureAtlas, nameSubTexture, nameUsage, glm::vec3(1.f));
    //         }

    //         if (!sprite)
    //         {
    //             continue;
    //         }

    //         auto framesIterator = currentSpite.FindMember("frames");
    //         if (framesIterator != currentSpite.MemberEnd())
    //         {
    //             const auto framesArray = framesIterator->value.GetArray();
    //             std::vector<RenderEngine::Sprite2D::FrameDescription> framesDescription;
    //             framesDescription.reserve(framesArray.Size());
    //             for (const auto &currentFrame : framesArray)
    //             {
    //                 const std::string nameSubTexture = currentFrame["subTexture"].GetString();
    //                 const double duration = currentFrame["duration"].GetDouble();
    //                 const auto textureAtlas = get_texture2D(nameTextureAtlas);
    //                 const auto subTexture = textureAtlas->get_sub_texture(nameSubTexture);
    //                 framesDescription.emplace_back(subTexture.leftBottomVertex, subTexture.rightTopVertex, duration);
    //             }
    //             sprite->insert_frames(std::move(framesDescription));
    //         }
    //     }
    // }

    auto soundIterator = document.FindMember("sounds");
    if (soundIterator != document.MemberEnd())
    {
        for (const auto &currentSound : soundIterator->value.GetArray())
        {
            const std::string name = currentSound["name"].GetString();
            const std::string soundPath = currentSound["soundPath"].GetString();

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