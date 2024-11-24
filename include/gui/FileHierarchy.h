#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"

class FileHierarchy {
    private:
        ImVec2 size, pos;
    public:
    void ShowFileHierarchy(GameEngine *gameEngine, const std::vector<std::shared_ptr<GameObject>>& Objects, bool &showAddObject);
    void SetParms(ImVec2 _size, ImVec2 _pos) {
        size = _size;
        pos = _pos;
    };
};
