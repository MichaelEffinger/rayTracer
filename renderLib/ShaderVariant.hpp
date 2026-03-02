#pragma once
#include "Shader.hpp"

namespace ES {

    using Shader = std::variant <
        ConstantShader,
        RandomShader,
        NormalShader,
        LambertianShader,
        BlinnPhongShader,
        MirrorShader,
        DiffuseShader
    >;

    namespace shader {
        RGB shade(const Shader& shader_, const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& shapes) noexcept;
    }
}