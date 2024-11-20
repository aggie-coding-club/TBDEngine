#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"
#include <glm/common.hpp>

class AddObjectWindow{
  public:
    void showAddObjectWindow(GameEngine *engine, bool &ShowAddObject, int &objectType);
};