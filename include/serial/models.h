#pragma once
#include <yaml-cpp/yaml.h>
#include "../core/game_engine.h"
#include "components/components.h"

YAML::Node SerializeModels(std::vector<std::shared_ptr<GameObject>> models);