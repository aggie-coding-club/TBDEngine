#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "game_object.h"
#include "components/material.h"
#include "components/transform.h"

class GameEngine {
private:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
public:
    void addObjectToVerticesMap(std::shared_ptr<GameObject> gameObject);

    void test_init()
    {
        auto bunnyObj = std::make_shared<GameObject>();
        auto bunnyTransform = std::make_shared<Transform>();

        bunnyObj->components.push_back(bunnyTransform);

        gameObjects.push_back(bunnyObj);
    }

    void test_init_2()
    {
        size_t n = 3;
        glm::vec3 pos[n] = {
            {0.0f, -1.0f, 0.0f},
            {-2.0f, -1.0f, -3.0f},
            {2.0f, -1.0f, -3.0f}
        };
        for (int i = 0; i < n; i++)
        {
            auto bunnyObj = std::make_shared<GameObject>();
            auto bunnyTransform = std::make_shared<Transform>(pos[i]);

            bunnyObj->components.push_back(bunnyTransform);

            gameObjects.push_back(bunnyObj);

        }

    }

    void test_init_3() {
        size_t n = 3;
        glm::vec3 pos[n] = {
            {0.0f, -1.0f, 0.0f},
            {-2.0f, -1.0f, -3.0f},
            {2.0f, -1.0f, -3.0f}
        };
        for (int i = 0; i < n; i++)
        {
            auto bunnyObj = std::make_shared<GameObject>();
            auto bunnyName = std::string("bunny" + std::to_string(i));
            auto bunnyTransform = std::make_shared<Transform>(pos[i]);
            auto bunnyMaterial = std::make_shared<Material>();

            bunnyObj->name = bunnyName;
            bunnyObj->components.push_back(bunnyTransform);
            bunnyObj->components.push_back(bunnyMaterial);

            gameObjects.push_back(bunnyObj);

        }
    }

    // Test Constructor
    GameEngine()
    {
        test_init_3();
    }

    const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return gameObjects; }

};
