#include "physics/physics_engine.h"
#include "core/components/rigidbody.h"

void PhysicsEngine::Update() {

}

void PhysicsEngine::ApplyGravity() {
    const auto& scene = gameEngine->GetCurrScene();
    for (const auto& model : scene->GetModels())
    {
        const auto& objRigidBody = std::dynamic_pointer_cast<RigidBody>( 
                model->components[RIGID_BODY] 
        );

        if (objRigidBody == nullptr)
            continue;
        
        if (! objRigidBody->useGravity)
            continue;
    }
        
}
