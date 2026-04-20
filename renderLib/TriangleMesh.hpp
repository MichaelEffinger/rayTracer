#include <vector>
#include "Triangle.hpp"

namespace ES{
    class TriangleMesh{

        public:

        std::vector<Triangle> triangles;



        constexpr TriangleMesh(std::vector<float> vertices) noexcept{
            for (std::size_t i = 0; i + 8 < vertices.size(); i += 9) {
                triangles.push_back(Triangle{
                    {vertices[i],     vertices[i+1],     vertices[i+2]},
                    {vertices[i+3],   vertices[i+4],     vertices[i+5]},
                    {vertices[i+6],   vertices[i+7],     vertices[i+8]}
                });
            }
        }
        constexpr bool intersect(Ray<float> ray, float t_min, float& t_max, Hit& hitstruct) const noexcept {
            bool hit = false;
            float tClosest = t_max;

            Hit tempHit;

            for (const auto& tri : triangles) {
                float t;
                if (tri.intersect(ray, t_min, tClosest, tempHit)) {
                    tClosest = tempHit.t;
                    hitstruct = tempHit;
                    hit = true;
                }
            }

            t_max = tClosest;
            return hit;
        }


        constexpr VectorN<float,3> normal(const Hit& hit) const noexcept {
            return hit.normal;
        }

    };
}