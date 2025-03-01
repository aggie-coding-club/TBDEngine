#pragma once

#include "component.h"
#include <glm/glm.hpp>


class RigidBody: public Component 
{
public:
    // properties of rigid body
    glm::vec3 centerOfMass;
    glm::vec3 velocity;
    glm::vec3 angularVelocity;
    glm::vec3 maxVelocity = {7, 7, 7};
    glm::vec3 maxAngularVelocity = {2, 2, 2};
    glm::vec3 gravity = {0, 0, -9.8};
    float mass;

    // flags
    bool autoCenterOfMass = true;
    bool detectCollisions = true;
    bool useGravity = false;
    // If false, no force, gravity, or collision will affect the body
    bool isKinematics = true;

    /* Publics functions to be implemented later
     * AddForce()
     * MovePosition()
     * MoveRotation()
     */
    
private:
    // Copy Transform values here
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};