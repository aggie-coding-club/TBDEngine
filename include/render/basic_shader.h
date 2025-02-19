#pragma once

#include "render/shader.h"

class Basic_Shader : public Shader{

public:

    void UpdateData() {
        this->SendUniformData(1.f,"tempcolor");
    };
};