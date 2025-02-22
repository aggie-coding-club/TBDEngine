#pragma once

#include "imgui.h"
#include "core/game_engine.h"

class MenuBar{
  public:
    void ShowMenuBar(GameEngine *engine, bool &ShowDetail, bool &ShowView, bool &ShowHierarchy, bool &ShowCameraDebug, bool &ShowLoadFile, bool &ShowSaveAs);
};