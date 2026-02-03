#pragma once
#include <variant>
#include "PointN.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"


class Triangle{
    public:

    bool intersect(ES::Ray<float> r, float& hit) const{
        return false;
    }


    ES::VectorN<float,3> normal(ES::PointN<float,3> hit_point){
        return {0,0,0};
    }
};

namespace ES{
    using Shape = std::variant<Sphere, Plane, Triangle>;


    
    namespace shape{

        inline bool intersect(Shape& s,Ray<float>& r, float& t_hit) {
            bool hit = false;
            std::visit([&](const auto& obj){ hit = obj.intersect(r, t_hit);}, s);
            return hit;

        }

        inline Vector3<float> normal(Shape& s, PointN<float, 3> hit_point) noexcept{
            return std::visit([&](auto&& obj)-> Vector3<float> {return obj.normal(hit_point);},s);
        }
    }
}