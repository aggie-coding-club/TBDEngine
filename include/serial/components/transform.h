#pragma once
#include <yaml-cpp/yaml.h>
#include "core/components/transform.h"


YAML::Node SerializeTransform(const std::shared_ptr<Transform>& transform);