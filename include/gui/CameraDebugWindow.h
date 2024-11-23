#pragma once

#include "imgui.h"
#include "core/camera.h"
#include <glm/common.hpp>

#include <memory>

class CameraDebugWindow{
  public:
    void ShowCameraDebugWindow(std::shared_ptr<Camera> camera);
};