#include <yaml-cpp/yaml.h>
#include "core/camera.h"

YAML::Node SerializeCameras(const std::vector<std::shared_ptr<Camera>>& cameras);

