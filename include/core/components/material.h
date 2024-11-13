#pragma once

#include "component.h"
#include <glm/vec3.hpp>

class Material: public Component {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shinniness;

    public:
        Material(const glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f),
                 const glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
                 const glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f),
                 const float shinniness = 32.0f)
            : ambient(ambient), diffuse(diffuse), specular(specular), shinniness(shinniness)
        {
            type = MATERIAL;
        }
};