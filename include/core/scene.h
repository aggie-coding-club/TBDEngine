#pragma once

#include "core/game_object.h"
#include "core/camera.h"
#include "core/components/transform.h"
#include "core/components/model.h"
#include "core/components/material.h"

#include <vector>

class Scene {
private:
    std::vector<std::shared_ptr<GameObject>> 	models;
    std::vector<std::shared_ptr<GameObject>> 	lights;
    std::vector<std::shared_ptr<Camera>> 		cameras;

    std::string name;
    int currCameraIdx = 0;

    std::string modelPath = "../resources/models/";

    static int randomId;
public:
    std::shared_ptr<GameObject> selectedGameObj = nullptr;

    Scene() = default;
  	std::vector<std::shared_ptr<GameObject>>& 	GetModels()
    {
        return models;
    }
    std::vector<std::shared_ptr<GameObject>>& 	GetLights()
    {
        return lights;
    }
    std::shared_ptr<Camera>& GetCurrCamera()
    {
        return cameras[currCameraIdx];
    }
    void AddModel(const std::shared_ptr<GameObject>& model)
    {
        models.push_back(model);
    }

    void AddModel(){
        glm::vec3 pos= {0.0f, 0.0f, 0.0f};

        const auto bunnyObj = std::make_shared<GameObject>();
        const auto bunnyName = std::string("bunny" + std::to_string(randomId++));
        const auto bunnyTransform = std::make_shared<Transform>(pos);
        const auto bunnyMaterial = std::make_shared<Material>();
  	    const auto bunnyModel = std::make_shared<Model>();

        bunnyObj->name = bunnyName;
        bunnyObj->components[TRANSFORM] = bunnyTransform;
        bunnyObj->components[MATERIAL] = bunnyMaterial;
  	    bunnyObj->components[MODEL] = bunnyModel;

        AddModel(bunnyObj);
    };
};