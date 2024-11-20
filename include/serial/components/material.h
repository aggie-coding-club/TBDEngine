#pragma once
#include <yaml-cpp/yaml.h>
#include "core/components/material.h"

YAML::Node SerializeMaterial(std::shared_ptr<Material> material);