#pragma once

#include "component.h"
#include <glm/glm.hpp>


class RigidBody: public Component 
{
    public:
        Vec3 centerOfMass;
        Vec3 angularVelocity;
        bool autoCenterOfMass;
}