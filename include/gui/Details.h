#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"
#include <iostream>

class Details{
    private:
        ImVec2 size;
        ImVec2 pos;
    public:
        void ShowDetails(std::shared_ptr<GameObject> object);
};