#pragma once

#include <string>

enum COMPONENT_TYPE {
    NONE,
    TRANSFORM,
    MATERIAL,
    MODEL,
    LIGHT,
};

class Component {
private:

public:
    COMPONENT_TYPE type;
    virtual ~Component() = default;
    Component(): type(NONE) {}
    virtual COMPONENT_TYPE GetType() { return type; };
};