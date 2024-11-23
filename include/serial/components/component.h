#pragma once
#include <yaml-cpp/yaml.h>
#include "core/components/component.h"

YAML::Node SerializeComponent(const std::array<std::shared_ptr<Component>, Component::GetEnumSize()>& components);