#pragma once
#include <yaml-cpp/yaml.h>
#include "core/components/transform.h"


YAML::Node SerializeTransform(const std::shared_ptr<Transform>& transform);


// I've written a good bit of the serialization but I'm trying to test it so I'm just messing around with trying to output a file in main.cpp but when I write this code I get an error after make -j8
// #include <yaml-cpp/yaml.h>
// YAML::Node node;
// node["key"] = "value"