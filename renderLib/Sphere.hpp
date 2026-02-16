#pragma once
#include <cmath>
#include "ES_math.hpp"
#include "Ray.hpp"
#include "PointN.hpp"
#include "Shader.hpp"
#include "Hit.hpp"

namespace ES {

class Sphere {
public:
    PointN<float, 3> origin;
    float radius;

    constexpr Sphere() noexcept = default;

    constexpr Sphere(PointN<float,3> newOrigin, float newRadius) noexcept
        : origin(newOrigin), radius(newRadius){}


    constexpr bool intersect(Ray<float> ray, float t_min,  float& t_max, Hit& hitstruct) const noexcept {
        VectorN<float,3> oc = ray.origin - origin;
        float a = ray.direction.dot(ray.direction);
        float half_b = ray.direction.dot(oc);
        float c = oc.dot(oc) - radius*radius;
        
        float discriminant = half_b*half_b - a*c;

        if (discriminant < 0) {
            return false;
        }

        float sqrt_d = std::sqrt(discriminant);
        float t0 = -half_b - sqrt_d;
        float t1 = -half_b + sqrt_d;

        float t = (-half_b - sqrt_d) / a;
        if (t < t_min || t > t_max) {
            t = (-half_b + sqrt_d) / a;
            if (t < t_min || t > t_max){
                return false;
            }
        }

        t_max = t;
        hitstruct.t = t;
        hitstruct.position = ray.at(hitstruct.t);
        VectorN<float,3> outward_normal = (hitstruct.position - origin)/radius;
        hitstruct.normal = (hitstruct.position - origin)/radius;
        hitstruct.set_face_normal(ray,outward_normal);

        return true;
    }

    constexpr VectorN<float,3> normal(Hit& hit) const noexcept {
        return (hit.position - origin).normalize();
    }
};

}
