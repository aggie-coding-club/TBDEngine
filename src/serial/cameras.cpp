#include "serial/cameras.h"

YAML::Node SerializeCameras(const std::vector<std::shared_ptr<Camera>>& cameras) {
    auto CamerasNode = YAML::Node();

    CamerasNode["Cameras"];

    for(const auto& camera: cameras) {
        auto CurrCameraNode = YAML::Node();

        // CurrCameraNode["fovy"] = camera->GetFovy();
        // CurrCameraNode["aspect"] = camera->GetAspect();

        // glm::vec3 center = camera->GetCenter();
        // CurrCameraNode["center"] = std::vector<float>(center[0], center[1], center[2]);

        // glm::vec3 eye = camera->GetEye();
        // CurrCameraNode["eye"] = std::vector<float>(eye[0], eye[1], eye[2]);

        // glm::vec3 up = camera->GetUp();
        // CurrCameraNode["up"] = std::vector<float>(up[0], up[1], up[2]);

        CamerasNode["Cameras"] = CurrCameraNode;
    }

    return CamerasNode;
}