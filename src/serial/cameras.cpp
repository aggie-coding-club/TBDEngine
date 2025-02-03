#include "serial/cameras.h"

YAML::Node SerializeCameras(const std::vector<std::shared_ptr<Camera>>& cameras) {
    auto CamerasNode = YAML::Node();

    CamerasNode["Cameras"];

    for(const auto& camera: cameras) {
        auto CurrCameraNode = YAML::Node();

        CurrCameraNode["fovy"] = camera->GetFovy();
        CurrCameraNode["aspect"] = camera->GetAspect();

        glm::vec3 center = camera->GetCenter();
        CurrCameraNode["center"] = std::vector<float>{center[0], center[1], center[2]};

        glm::vec3 eye = camera->GetEye();
        CurrCameraNode["eye"] = std::vector<float>{eye[0], eye[1], eye[2]};

        glm::vec3 up = camera->GetUpVec();
        CurrCameraNode["up"] = std::vector<float>{up[0], up[1], up[2]};


        CamerasNode["Camera Name"] = CurrCameraNode;
        // Change "Camera Name"
    }

    return CamerasNode;
}

void DeserializeCameras(std::vector<std::shared_ptr<Camera>>& cameras, const YAML::Node& camerasNode) {
    for(const auto& currCameraNode : camerasNode) {
        const auto camera = std::make_shared<Camera>();
        std::string cameraName;

        for(const auto& cameraPair : currCameraNode) {
            cameraName = cameraPair.first.as<std::string>();
            const YAML::Node cameraNode = cameraPair.second;

            camera->SetFovy(cameraNode["fovy"].as<float>());
            camera->SetAspect(cameraNode["aspect"].as<float>());
            camera->SetCenter(glm::vec3(cameraNode["center"][0].as<float>(), cameraNode["center"][1].as<float>(), cameraNode["center"][2].as<float>()));
            camera->SetEye(glm::vec3(cameraNode["eye"][0].as<float>(), cameraNode["eye"][1].as<float>(), cameraNode["eye"][2].as<float>()));
            camera->SetUpVec(glm::vec3(cameraNode["up"][0].as<float>(), cameraNode["up"][1].as<float>(), cameraNode["up"][2].as<float>()));

        }
        cameras.push_back(camera);
    }
}