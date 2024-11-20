#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

#include "components/component.h"
class GameObject {
public:
    std::string name;
    std::vector<std::shared_ptr<Component>> components;
    std::unordered_set<std::string> tags;
    std::string model_path;

    GameObject(const std::string model_path = "../resources/models/bunny.obj") : model_path(model_path), id(generateUniqueId()) {}
private:
    const int id;

    static int lastId;

    static int generateUniqueId() {
        return lastId++;
    }
};
