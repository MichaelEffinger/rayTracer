#pragma once
#include "Shader.hpp"

namespace ES {

    using Shader = std::variant <
        ConstantShader,
        RandomShader,
        NormalShader,
        LambertianShader,
        BlinnPhongShader,
        RandomLambertianShader,
        MirrorShader
    >;

    namespace shader {
        RGB shade(const Shader& shader_, const Hit& hit, const Ray<float>& ray,
                  const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& shapes) noexcept;
    }
}