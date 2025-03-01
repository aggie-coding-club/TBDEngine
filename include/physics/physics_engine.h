#pragma once
#include "core/game_engine.h"

class PhysicsEngine {
public:
    PhysicsEngine(std::chrono::duration<float> *timeDelta) 
        : timeDelta(timeDelta) {}
                                                             
    GameEngine* gameEngine; 
    std::chrono::duration<float> *timeDelta; 
    
    bool isActive = false;

    void Update();
private:
    void ApplyGravity();
};
