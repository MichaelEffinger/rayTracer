#include "PointN.hpp"
#include "VectorN.hpp"
#include "Ray.hpp"
namespace ES{
    struct Plane {
        PointN<float,3> point;
        VectorN<float,3> n;

        bool intersect(const Ray<float>& ray, float& t_hit) const noexcept {
            float denom = n.dot(ray.direction);
            if (std::abs(denom) < 1e-6f) return false;

            float t = (point - ray.origin).dot(n) / denom;
            if (t > 1e-6f) {
                t_hit = t;
                return true;
            }
            return false;
        }

        VectorN<float,3> normal(const PointN<float,3>&) const noexcept {
            return n;
        }
    };
}