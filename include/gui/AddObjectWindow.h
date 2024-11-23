#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"
#include <glm/common.hpp>

class AddObjectWindow{
    private:
        int objectType = 0;
        std::string location;
        std::string name;
  public:
    void showAddObjectWindow(GameEngine *engine, bool &ShowAddObject);
};