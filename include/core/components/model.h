#pragma once

#include <glm/vec3.hpp>
#include "component.h"
#include <string>

class Model: public Component {
public:
    std::string modelPath;

    // TODO Remove hardcoded bunny
    Model(const std::string& modelPath = "../resources/models/bunny.obj")
        : modelPath(modelPath)
    {
        type = MODEL;
    }

    // Add function to process component

};
