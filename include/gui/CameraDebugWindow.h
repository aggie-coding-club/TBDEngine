#pragma once

#include "imgui.h"
#include "core/camera.h"
#include <glm/common.hpp>

class CameraDebugWindow{
  public:
    void ShowCameraDebugWindow(Camera *camera);
};