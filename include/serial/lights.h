#pragma once
#include <yaml-cpp/yaml.h>
#include "core/game_engine.h"
#include "core/game_object.h"

YAML::Node SerializeLights(const std::vector<std::shared_ptr<GameObject>>& lights);
