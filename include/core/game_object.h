#pragma once

#include <string>
#include <array>
#include <unordered_set>
#include <memory>

#include "components/component.h"
class GameObject {
public:
    std::string name;
    std::array<std::shared_ptr<Component>, Component::GetEnumSize()> components{};
    std::unordered_set<std::string> tags;

    GameObject() : id(generateUniqueId()) {}
private:
    const int id;

    static int lastId;

    static int generateUniqueId() {
        return lastId++;
    }
};
