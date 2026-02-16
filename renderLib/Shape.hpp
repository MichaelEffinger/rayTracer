#pragma once
#include <variant>
#include "PointN.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Triangle.hpp"

namespace ES{
    using Shape = std::variant<Sphere, Plane, Triangle>;


    
    namespace shape{

        inline bool intersect(const Shape& s,Ray<float>& r, float t_min, float& t_max, Hit& hitstruct) {
            bool hit = false;
            std::visit([&](const auto& obj){ hit = obj.intersect(r, t_min, t_max, hitstruct);}, s);
            return hit;

        }

        inline Vector3<float> normal(const Shape& s, Hit &hit) noexcept{
            return std::visit([&](auto&& obj)-> Vector3<float> {return obj.normal(hit);},s);
        }
    }
}