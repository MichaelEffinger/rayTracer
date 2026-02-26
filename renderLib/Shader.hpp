#pragma once
#include <variant>
#include <random>
#include "ColorN.hpp"
#include "ES_math.hpp"
#include "Ray.hpp"
#include "VectorN.hpp"
#include "PointLight.hpp"
#include "Hit.hpp"

namespace ES {

class ShapeList; // forward declare

class ConstantShader {
public:
    RGB color;
    ConstantShader(RGB newColor);
    constexpr RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept;
};

class RandomShader {
public:
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<float> dist;
    RandomShader();
    RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept;
};

class LambertianShader {
public:
    RGB color;
    float intensity;
    LambertianShader(RGB newColor, float intensity_);
    constexpr RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept;
};

class RandomLambertianShader {
public:
    RGB color;
    float intensity;
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<float> dist;
    RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept;
};

class NormalShader {
public:
    NormalShader();
    constexpr RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept;
};

class BlinnPhongShader {
public:
    RGB color;
    float shininess;
    float intensity;
    RGB specular_color;
    BlinnPhongShader(RGB color_, float shininess_, float intensity_, RGB specular_color_ = {1,1,1}) noexcept;
    RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept;
};

class MirrorShader {
public:
    MirrorShader() noexcept;
    RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept;
};

} // namespace ES