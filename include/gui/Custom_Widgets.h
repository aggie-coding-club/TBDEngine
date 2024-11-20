#pragma once
#include "core/game_engine.h"
#include "core/camera.h"

// Custom widgets
void ShowFileHierarchy(GameEngine *gameEngine, const std::vector<std::shared_ptr<GameObject>>& Objects, bool &ShowAddObject);
void ShowMenuBar(bool &ShowDetail, bool &ShowView, bool &ShowHierarchy, bool &ShowCameraDebug);
void ShowViewport(ImVec2 window_Size);
void ShowDetails(std::shared_ptr<GameObject> object);
void ShowCameraDebugWindow(Camera* camera);
void ShowAddObjectWindow(GameEngine *engine, bool &ShowAddObject, int &objectType);