#pragma once

enum COMPONENT_TYPE {
    TRANSFORM,
    MATERIAL,
    MODEL,
    LIGHT,
    NUM_ENUM
};

class Component {
private:

public:
    COMPONENT_TYPE type;
    virtual ~Component() = default;
    Component(): type(NUM_ENUM) {}
    virtual COMPONENT_TYPE GetType() { return type; };

    static constexpr size_t GetEnumSize() { return NUM_ENUM; };
};