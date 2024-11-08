#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "game_object.h"
#include "components/transform.h"

class GameEngine {
private:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
public:
    void addObjectToVerticesMap(std::shared_ptr<GameObject> gameObject);

    void test_init()
    {
        auto bunnyObj = std::make_shared<GameObject>();
        Transform bunnyTransform;
        bunnyObj->components.push_back(bunnyTransform);

        gameObjects.push_back(bunnyObj);
    }

    // Test Constructor
    GameEngine()
    {
        test_init();

    }

};
