#pragma once

#include "component.h"
#include <glm/vec3.hpp>


class RigidBody: public Component 
{
    public:
        Vector3 centerOfMass;
        Vector3 angularVelocity;
        bool autoCenterOfMass;
}