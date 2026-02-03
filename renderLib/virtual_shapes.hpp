#pragma once
#include <cmath>
#include <memory>
#include "ColorN.hpp"
#include "Ray.hpp"
#include "PointN.hpp"
#include "VectorN.hpp"
using ES::RGB;
using ES::PointN;
using ES::VectorN;
using ES::Ray;

namespace slow {

// -----------------------------------------------------------
// Shader
// -----------------------------------------------------------
class IShader {
public:
    virtual ~IShader() = default;
    virtual RGB shade(const PointN<float,3>& hit_point, const VectorN<float,3>& normal, const Ray<float>& ray) const noexcept = 0;
};

class ConstantShader : public IShader {
public:
    RGB color;

    ConstantShader(RGB newColor) : color(newColor) {}

    RGB shade(const PointN<float,3>& hit_point, const VectorN<float,3>& normal, const Ray<float>& ray) const noexcept override {
        return color;
    }
};

// -----------------------------------------------------------
// Shape (owns its shader)
// -----------------------------------------------------------
class IShape {
public:
    std::unique_ptr<IShader> shader;

    IShape(std::unique_ptr<IShader> shader_) : shader(std::move(shader_)) {}
    virtual ~IShape() = default;

    virtual bool intersect(const Ray<float>& ray, float& t_hit) const noexcept = 0;
    virtual VectorN<float,3> normal(const PointN<float,3>& hit_point) const noexcept = 0;
};

class Sphere : public IShape {
public:
    PointN<float, 3> origin;
    float radius;

    Sphere(PointN<float,3> newOrigin, float newRadius, std::unique_ptr<IShader> shader_) noexcept
        : IShape(std::move(shader_)), origin(newOrigin), radius(newRadius) {}

    bool intersect(const Ray<float>& ray, float& t_hit) const noexcept override {
        VectorN<float,3> oc = ray.origin - origin;
        float a = ray.direction.dot(ray.direction); // should be 1 if normalized
        float half_b = ray.direction.dot(oc);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;

        float sqrt_d = std::sqrt(discriminant);
        float t0 = -half_b - sqrt_d;
        float t1 = -half_b + sqrt_d;

        const float epsilon = 1e-6f;

        if (t0 > epsilon) { t_hit = t0; return true; }
        if (t1 > epsilon) { t_hit = t1; return true; }
        return false;
    }

    VectorN<float,3> normal(const PointN<float,3>& hit_point) const noexcept override {
        return (hit_point - origin).normalize();
    }
};

// -----------------------------------------------------------
// SceneObject
// -----------------------------------------------------------
class SceneObject {
public:
    std::unique_ptr<IShape> obj; // shader lives inside obj

    SceneObject(std::unique_ptr<IShape> obj_) : obj(std::move(obj_)) {}

    SceneObject(SceneObject&&) noexcept            = default;
    SceneObject& operator=(SceneObject&&) noexcept = default;
};

class Plane : public IShape {
public:
    PointN<float,3> point;     // any point on the plane
    VectorN<float,3> n;        // plane normal (normalized)

    Plane(PointN<float,3> p,
          VectorN<float,3> normal,
          std::unique_ptr<IShader> shader_) noexcept
        : IShape(std::move(shader_)),
          point(p),
          n(normal.normalize()) {}

    bool intersect(const Ray<float>& ray, float& t_hit) const noexcept override {
        const float epsilon = 1e-6f;

        float denom = n.dot(ray.direction);
        if (std::fabs(denom) < epsilon)
            return false; // ray parallel to plane

        float t = (point - ray.origin).dot(n) / denom;
        if (t > epsilon) {
            t_hit = t;
            return true;
        }
        return false;
    }

    VectorN<float,3> normal(const PointN<float,3>&) const noexcept override {
        return n;
    }
};


} // namespace ES


