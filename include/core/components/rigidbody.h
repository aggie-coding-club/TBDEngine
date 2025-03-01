#pragma once

#include "component.h"
#include <glm/glm.hpp>


class RigidBody: public Component 
{
    public:
        glm::vec3 centerOfMass;
        glm::vec3 angularVelocity;
        bool autoCenterOfMass;
};