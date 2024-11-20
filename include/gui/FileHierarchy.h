#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"

class FileHierarchy {
    public:
    void ShowFileHierarchy(GameEngine *gameEngine, const std::vector<std::shared_ptr<GameObject>>& Objects, bool &showAddObject);
};
