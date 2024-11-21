#include "serial/project.h"
#include "serial/scenes.h"

void SerializeProject(GameEngine* engine) {
    auto root  = YAML::Node();
    root["Project Name"] = "Name";
    root["Scenes"] = SerializeScenes(engine->GetScenes());
}
