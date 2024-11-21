#pragma once
#include <yaml-cpp/yaml.h>
#include "core/scene.h"

YAML::Node SerializeScenes(const std::vector<std::shared_ptr<Scene>>& scenes);