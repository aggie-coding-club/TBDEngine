#pragma once
#include <yaml-cpp/yaml.h>
#include "core/game_engine.h"

YAML::Node SerializeProject(GameEngine* engine);