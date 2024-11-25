#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"
#include <iostream>

class Details{
    private:
        ImVec2 size, pos;
    public:
        void ShowDetails(const std::shared_ptr<Scene>& scene);
        void SetParms(ImVec2 _size, ImVec2 _pos) {
            size = _size;
            pos = _pos;
        };
};