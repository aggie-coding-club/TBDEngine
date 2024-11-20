#include "serial/components/component.h"
#include "serial/components/transform.h"
#include "serial/components/material.h"
#include "serial/components/model.h"
#include "serial/components/light.h"

YAML::Node SerializeComponent(const std::array<std::shared_ptr<Component>, Component::GetEnumSize()>& components) {
    auto ComponentsNode = YAML::Node();

    for(const auto& component: components) {
        switch(component->type)
        {
        case TRANSFORM:
        {
            const auto& transform = std::dynamic_pointer_cast<Transform>(component);
            ComponentsNode["Transform"] = SerializeTransform(transform);
            break;

        }
        case MATERIAL:
        {
            const auto& material = std::dynamic_pointer_cast<Material>(component);
            ComponentsNode["Material"] = SerializeMaterial(material);
            break;
        }
        case MODEL:
        {
            const auto& model = std::dynamic_pointer_cast<Model>(component);
            ComponentsNode["Material"] = SerializeModel(model);
            break;
        }
        case LIGHT:
        {
            const auto& light = std::dynamic_pointer_cast<Light>(component);
            ComponentsNode["Material"] = SerializeLight(light);
            break;
        }
        default:
            break;
        }
    }

    return ComponentsNode;
}