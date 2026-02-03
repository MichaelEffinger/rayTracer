#pragma once
#include <cmath>
#include "Ray.hpp"
#include "PointN.hpp"
#include "Shader.hpp"

namespace ES {

class Sphere {
public:
    PointN<float, 3> origin;
    float radius;

    constexpr Sphere(PointN<float,3> newOrigin, float newRadius) noexcept
        : origin(newOrigin), radius(newRadius){}

    // returns true if ray hits sphere, sets t_hit
    constexpr bool intersect(const Ray<float>& ray, float& t_hit) const noexcept {
        VectorN<float,3> oc = ray.origin - origin;
        float a = ray.direction.dot(ray.direction); // should be 1 if normalized
        float half_b = ray.direction.dot(oc);
        float c = oc.dot(oc) - radius*radius;
        float discriminant = half_b*half_b - a*c;

        if (discriminant < 0) return false;

        float sqrt_d = std::sqrt(discriminant);
        float t0 = -half_b - sqrt_d;
        float t1 = -half_b + sqrt_d;

        const float epsilon = 1e-6f;

        if (t0 > epsilon) { t_hit = t0; return true; }
        if (t1 > epsilon) { t_hit = t1; return true; }
        return false;
    }

    constexpr VectorN<float,3> normal(const PointN<float,3>& hit_point) const noexcept {
        return (hit_point - origin).normalize();
    }
};

}
