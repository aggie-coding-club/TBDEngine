#pragma once


class RigidBody: public Component 
{
    public:
        Vec3 centerOfMass;
        Vec3 angularVelocity;
        bool autoCenterOfMass;
        
}