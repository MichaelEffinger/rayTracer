#pragma once
#include "Shader.hpp"

namespace ES {

    using Shader = std::variant <
        NormalShader,
        ConstantShader,
        RandomShader,
        LambertianShader,
        BlinnPhongShader,
        MirrorShader,
        DiffuseShader
    >;

    namespace shader {
        RGB shade(const Shader& shader_, const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& shapes) noexcept;
    }
}