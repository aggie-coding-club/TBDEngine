#include "component.h"
#include <glm/vec3.hpp>

class Model: public Component {
public:
    std::string path;
    Model (const std::string& path) : path(path) {}
};
