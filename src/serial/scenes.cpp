#include "serial/scenes.h"
#include "serial/models.h"
#include "serial/lights.h"
#include "serial/cameras.h"

YAML::Node SerializeScenes(const std::vector<std::shared_ptr<Scene>>& scenes) {
    auto ScenesNode = YAML::Node();

    for(const std::shared_ptr<Scene>& scene : scenes) {
        auto CurrSceneNode = YAML::Node();
        CurrSceneNode[scene->GetName()]["Models"] = SerializeModels(scene->GetModels());
        CurrSceneNode[scene->GetName()]["Lights"] = SerializeLights(scene->GetLights());

        // CurrSceneNode["Models"] = SerializeModels(scene->GetModels());
        // CurrSceneNode["Lights"] = SerializeLights(scene->GetLights());
        // CurrSceneNode = SerializeCameras(scene->GetCameras());

        ScenesNode.push_back(CurrSceneNode);
    }

    return ScenesNode;
}