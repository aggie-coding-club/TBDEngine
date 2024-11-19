#pragma once
#include "../../../include/serial/components/components.h"

using namespace YAML;

Node SerializeComponents(std::vector<std::shared_ptr<Component>> components) {
    Node ComponentsNode = Node();

    for(std::shared_ptr<Component> component: components) {
        if(component->GetType() == TRANSFORM) {
            auto transform = std::dynamic_pointer_cast<Transform>(component);
            if(transform) {
                ComponentsNode["Transform"] = SerializeTransform(transform);
            }
        }
        if(component->GetType() == MATERIAL) {
            auto material = std::dynamic_pointer_cast<Material>(component);
            if(material) {
                ComponentsNode["Material"] = SerializeMaterial(material);
            }
        }
    }

    return ComponentsNode;

}