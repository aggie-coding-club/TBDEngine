#include "project.h"
#include "scenes.h"

using namespace YAML;

void SerializeProject(GameEngine* engine) {
    Node root;
    root["Project Name: "] = "Name";
    root["Scenes"] = SerializeScenes(engine->scenes)
}