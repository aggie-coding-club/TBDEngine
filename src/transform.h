#include "component.h"
#include <glm/vec3.hpp>
using namespace glm;

class Transform: public Component {
public:
    dvec3 position;
    dvec3 rotation;
    dvec3 size;

    Transform(const dvec3 pos = dvec3(0.0, 0.0, 0.0),
              const dvec3 rot = dvec3(0.0, 0.0, 0.0),
              const dvec3 scl = dvec3(0.0, 0.0, 0.0))
        : position(pos), rotation(rot), size(scl) {}

};