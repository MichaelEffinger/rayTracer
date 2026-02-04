#pragma once
#include <cmath>
#include <memory>
#include <random>
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
    virtual RGB shade(const PointN<float,3>& hit_point, 
                      const VectorN<float,3>& normal, 
                      const Ray<float>& ray,
                      const VectorN<float,3>& light_direction) const noexcept = 0;
};

class ConstantShader : public IShader {
public:
    RGB color;
    
    ConstantShader(RGB newColor) : color(newColor) {}
    
    RGB shade(const PointN<float,3>& hit_point, 
              const VectorN<float,3>& normal, 
              const Ray<float>& ray,
              const VectorN<float,3>& light_direction) const noexcept override {
        return color;
    }
};

class RandomShader : public IShader {
public:
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<float> dist;
    
    RandomShader() : rng(std::random_device{}()), dist(0.0f, 1.0f) {}
    
    RGB shade(const PointN<float,3>& hit_point, 
              const VectorN<float,3>& normal, 
              const Ray<float>& ray,
              const VectorN<float,3>& light_direction) const noexcept override {
        return RGB{dist(rng), dist(rng), dist(rng)};
    }
};

class NormalShader : public IShader {
public:
    NormalShader() {}
    
    RGB shade(const PointN<float,3>& hit_point, 
              const VectorN<float,3>& normal, 
              const Ray<float>& ray,
              const VectorN<float,3>& light_direction) const noexcept override {
        return RGB{0.5f * (normal.x() + 1.f), 
                   0.5f * (normal.y() + 1.f), 
                   0.5f * (normal.z() + 1.f)};
    }
};

class LambertianShader : public IShader {
public:
    RGB color;
    float intensity;
    
    LambertianShader(RGB newColor, float intensity_) 
        : color(newColor), intensity(intensity_) {}
    
    RGB shade(const PointN<float,3>& hit_point, 
              const VectorN<float,3>& normal, 
              const Ray<float>& ray,
              const VectorN<float,3>& light_direction) const noexcept override {
        VectorN<float,3> L = -light_direction;
        float NdotL = std::max(0.f, normal.dot(L));
        RGB temp = color * intensity * NdotL;
        return temp.clamp(0, 1);
    }
};

class RandomLambertianShader : public IShader {
public:
    float intensity;
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<float> dist;
    
    RandomLambertianShader(float intensity_) 
        : intensity(intensity_), rng(std::random_device{}()), dist(0.0f, 1.0f) {}
    
    RGB shade(const PointN<float,3>& hit_point, 
              const VectorN<float,3>& normal, 
              const Ray<float>& ray,
              const VectorN<float,3>& light_direction) const noexcept override {
        VectorN<float,3> L = -light_direction;
        float NdotL = std::max(0.f, normal.dot(L));
        RGB temp = RGB{dist(rng), dist(rng), dist(rng)} * intensity * NdotL;
        return temp.clamp(0, 1);
    }
};

class BlinnPhongShader : public IShader {
public:
    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shininess;
    float intensity;
    
    BlinnPhongShader(RGB ambient_, RGB diffuse_, RGB specular_, float shininess_, float intensity_)
        : ambient(ambient_), diffuse(diffuse_), specular(specular_), 
          shininess(shininess_), intensity(intensity_) {}
    
    RGB shade(const PointN<float,3>& hit_point, 
              const VectorN<float,3>& normal, 
              const Ray<float>& ray,
              const VectorN<float,3>& light_direction) const noexcept override {
        VectorN<float,3> N = normal.normalize();
        VectorN<float,3> L = -light_direction.normalize();
        VectorN<float,3> V = (-ray.direction).normalize();
        VectorN<float,3> H = (L + V).normalize();
        
        float NdotL = std::max(0.f, N.dot(L));
        float NdotH = std::max(0.f, N.dot(H));
        
        RGB color = ambient + diffuse * (NdotL * intensity) + specular * std::pow(NdotH, shininess) * intensity;
        
        return color.clamp(0.f, 1.f);
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

} // namespace slow