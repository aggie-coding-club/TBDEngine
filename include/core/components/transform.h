#pragma once

#include "component.h"
#include <glm/vec3.hpp>

class Transform: public Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    std::string model_path;

    COMPONENT_TYPE type = TRANSFORM;

    // TODO Remove hardcoded bunny
    Transform(const glm::vec3 pos = glm::vec3(1.0f, 1.0f, 1.0f),
              const glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f), // In degrees
              const glm::vec3 scl = glm::vec3(1.0f, 1.0f, 1.0f))
        : position(pos), rotation(rot), scale(scl), model_path("../resources/bunny.obj") {}

    // Add function to process component

};