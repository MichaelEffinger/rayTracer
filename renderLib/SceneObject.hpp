#pragma once

#include "ShaderVariant.hpp"
#include "Shader.hpp"
#include "Shape.hpp"

namespace ES{
    class SceneObject{
    public:
        ShaderVariant shade;
        Shape obj;

        constexpr SceneObject(Shape obj_, ShaderVariant shade_): obj(obj_), shade(shade_) {}

        
    };
}