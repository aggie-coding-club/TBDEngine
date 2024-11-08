#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "components/component.h"
class GameObject {
public:
    std::string name;
    std::vector<Component> components;
    std::unordered_set<std::string> tags;

    GameObject() : id(generateUniqueId()) {}
private:
    const int id;

    static int lastId;

    static int generateUniqueId() {
        return lastId++;
    }
};

int GameObject::lastId = 0;