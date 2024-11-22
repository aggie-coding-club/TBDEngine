#include "serial/lights.h"
#include "serial/components/component.h"

YAML::Node SerializeLights(const std::vector<std::shared_ptr<GameObject>>& lights) {
    auto LightsNode = YAML::Node();

    for(const std::shared_ptr<GameObject>& light : lights) {
        auto CurrLightNode = YAML::Node();
        CurrLightNode = SerializeComponent(light->components);
        
        LightsNode[light->name] = CurrLightNode;
    }

    return LightsNode;
}
