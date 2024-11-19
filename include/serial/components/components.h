#pragma once
#include <yaml-cpp/yaml.h>
#include "../../include/core/components/component.h"
#include "transform.h"
#include "material.h"

YAML::Node SerializeComponents(std::vector<std::shared_ptr<Component>> components);