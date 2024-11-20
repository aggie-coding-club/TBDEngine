#pragma once
#include <yaml-cpp/yaml.h>
#include "../../core/components/component.h"
#include "../../core/components/transform.h"
#include "../../core/components/material.h"

YAML::Node SerializeComponents(std::vector<std::shared_ptr<Component>> components);