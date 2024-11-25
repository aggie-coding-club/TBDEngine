#include "serial/components/transform.h"

YAML::Node SerializeTransform(const std::shared_ptr<Transform>& transform) {
    auto TransformNode = YAML::Node();

    TransformNode["position"] = std::vector<float>{transform->position[0], transform->position[1], transform->position[2]};
    TransformNode["rotation"] = std::vector<float>{transform->rotation[0], transform->rotation[1], transform->rotation[2]};
    TransformNode["scale"] = std::vector<float>{transform->scale[0], transform->scale[1], transform->scale[2]};

    return TransformNode;
}