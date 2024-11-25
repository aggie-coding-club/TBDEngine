#include "serial/components/model.h"

YAML::Node SerializeModel(const std::shared_ptr<Model>& model) {
    auto ModelNode = YAML::Node();

    ModelNode["model_path"] = model->modelPath;

    return ModelNode;
}