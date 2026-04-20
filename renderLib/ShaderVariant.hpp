#pragma once
#include "Shader.hpp"

namespace ES {

    using ShaderVariant = std::variant <
        NormalShader,
        ConstantShader,
        RandomShader,
        LambertianShader,
        BlinnPhongShader,
        MirrorShader,
        DiffuseShader
    >;

    namespace shader {
        RGB shade(const ShaderVariant& shader_, const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& shapes) noexcept;
    }
}