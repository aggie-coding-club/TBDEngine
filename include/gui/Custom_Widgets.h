#pragma once
#include "core/game_engine.h"
#include "core/camera.h"

// Custom widgets
void ShowMenuBar(bool &ShowDetail, bool &ShowView, bool &ShowHierarchy, bool &ShowCameraDebug);
void ShowDetails(std::shared_ptr<GameObject> object);
void ShowCameraDebugWindow(Camera* camera);
void ShowAddObjectWindow(GameEngine *engine, bool &ShowAddObject, int &objectType);