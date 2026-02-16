#pragma once


#include "VectorN.hpp"
#include "PointN.hpp"
#include "Ray.hpp"

namespace ES{


    struct Hit{

        float t;
        float u = 0;
        float v = 0;
        bool front_face;


        PointN<float,3> position;
        VectorN<float,3> normal;

        void set_face_normal(const Ray<float>& ray, const VectorN<float,3>& outward_normal) {
            front_face = ray.direction.dot(outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }

    };

    

}