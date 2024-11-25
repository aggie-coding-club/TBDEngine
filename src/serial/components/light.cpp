#include "serial/components/light.h"

YAML::Node SerializeLight(const std::shared_ptr<Light>& light) {
    auto LightNode = YAML::Node();
    
    LightNode["color"] = std::vector<float>{light->color[0], light->color[1], light->color[2]};
    LightNode["intensity"] = light->intensity;

    return LightNode;
}