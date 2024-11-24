#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"
#include <iostream>

class Details{
    private:
        ImVec2 size, pos;
    public:
        void ShowDetails(std::shared_ptr<Scene> object, Camera* camera);
        void SetParms(ImVec2 _size, ImVec2 _pos) {
            size = _size;
            pos = _pos;
        };
};