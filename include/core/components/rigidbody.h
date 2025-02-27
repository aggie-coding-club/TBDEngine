#pragma once


class RigidBody: public Component 
{
    public:
        Vector3 centerOfMass;
        Vector3 angularVelocity;
        bool autoCenterOfMass;
        
}