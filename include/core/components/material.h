#pragma once

#include "component.h"
#include <glm/vec4.hpp>

struct MaterialInfo
{
    alignas(4) int textureIndex = -1;
    alignas(4) int normalIndex = -1;
    alignas(4) float emissionStrength;
    alignas(4) float specularProbability;
    alignas(4) float smoothness;
    alignas(4) float refractIndx;
    alignas(4) float transparency;
    alignas(16) glm::vec4 color;
    alignas(16) glm::vec4 emissionColor;
    alignas(16) glm::vec4 specularColor;
};

class Material: public Component {
    public:

        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;

        Material(const glm::vec4 ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1),
                 const glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1),
                 const glm::vec4 specular = glm::vec4(1.f, 0.f, 0.f, 1),
                 const float shininess = 32.0f)
            : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
        {
            type = MATERIAL;
        }

        MaterialInfo getMaterial()
        {
            MaterialInfo mat{-1,-1,
        0, 0.8, shininess, 0,0,
        diffuse, glm::vec4(0),specular};

            return mat;
        }
};