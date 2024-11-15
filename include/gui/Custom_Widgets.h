#pragma once
#include "core/game_engine.h"

// Custom widgets
void ShowFileHierarchy(GameEngine *gameEngine, const std::vector<std::shared_ptr<GameObject>>& Objects);
void ShowMenuBar(bool &ShowDetail, bool &ShowView, bool &ShowHierarchy);
void ShowViewport(ImVec2 window_Size);
void ShowDetails(std::shared_ptr<GameObject> object);