#include "PointN.hpp"
#include "VectorN.hpp"
#include "Ray.hpp"
#include "Hit.hpp"
namespace ES{
    struct Plane {
        PointN<float,3> point;
        VectorN<float,3> n;

        bool intersect(const Ray<float>& ray, float t_min, float& t_hit, Hit& hitstruct) const noexcept {
            return false;
        }

        VectorN<float,3> normal(Hit& hit) const noexcept {
            return n;
        }
    };
}