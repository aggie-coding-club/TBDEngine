#pragma once

#include "imgui.h"
#include "core/game_engine.h"
#include "core/camera.h"

class Details{
  public:
    void ShowDetails(std::shared_ptr<GameObject> object);
};