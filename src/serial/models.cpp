#include "serial/models.h"
#include "serial/components/component.h"

YAML::Node SerializeModels(const std::vector<std::shared_ptr<GameObject>>& models) {
    auto ModelsNode = YAML::Node();

    ModelsNode["Models"];

    for(const auto& model : models) {
        YAML::Node CurrModelNode;
        CurrModelNode = SerializeComponent(model->components);
        // can add things other than components to model data

        ModelsNode["Models"][model->name] = CurrModelNode;
    }

    return ModelsNode;
}