#pragma once

#include "ShaderVariant.hpp"
#include "Shader.hpp"
#include "Shape.hpp"

namespace ES{
    class SceneObject{
    public:
        Shader shade;
        Shape obj;

        constexpr SceneObject(Shape obj_, Shader shade_): obj(obj_), shade(shade_) {}

        
    };
}