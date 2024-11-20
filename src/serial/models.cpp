#pragma once
#include "../../include/serial/models.h"

using namespace YAML;

YAML::Node SerializeModels(std::vector<std::shared_ptr<GameObject>> models) {
    Node ModelsNode;

    ModelsNode["Models"] = Node();

    for(std::shared_ptr<GameObject> model : models) {
        Node CurrModelNode;
        CurrModelNode = SerializeComponents(model->components);
        // can add things other than components to model data

        ModelsNode["Models"][model->name] = CurrModelNode;
    }
}