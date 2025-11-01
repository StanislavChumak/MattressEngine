#ifndef REGITER_COMPONENT_H
#define REGITER_COMPONENT_H

#include "../../src/ECSWorld.h"
#include "../../src/resources/ResourceManager.h"

#include "rapidjson/document.h"
#include <functional>
#include <string>
#include <unordered_map>

struct TypeInfo {
    std::function<void(EntityID, const rapidjson::Value&,
    ECSWorld&, const ResourceManager&)> addComponentFromJson;
};

inline std::unordered_map<std::string, TypeInfo> typeRegistry;

template<typename T>
void registerComponent(const std::string& name) {
    typeRegistry[name] = {
        [](EntityID entity, const rapidjson::Value& json, 
            ECSWorld &world, const ResourceManager &resource) {
            T c;
            c.fromJson(json, resource);
            world.addComponent<T>(entity, std::move(c));
        }
    };
}

#endif
