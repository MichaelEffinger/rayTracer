#pragma once

#include "ES_math.hpp"
#include "PointN.hpp"
#include "Ray.hpp"
#include "Hit.hpp"

namespace ES{

    class Triangle {
        public: 
        PointN<float,3> A;
        PointN<float,3> B;
        PointN<float,3> C;
        VectorN<float,3> face_normal;
        
        constexpr Triangle() noexcept = default;

        constexpr Triangle(PointN<float,3> a, PointN<float,3> b, PointN<float,3> c) noexcept : A(a),B(b),C(c){
            VectorN<float,3> edge1 = B - A;
            VectorN<float,3> edge2 = C - A;
            face_normal = edge1.cross(edge2).normalize();
        }






        constexpr bool intersect(Ray<float> ray, float t_min, float t_max, Hit& hitstruct) const noexcept {
            VectorN<float,3> edge1 = B - A;
            VectorN<float,3> edge2 = C - A;

            VectorN<float,3> P = ray.direction.cross(edge2);
            float det = edge1.dot(P);

            constexpr float eps = math::default_epsilon<float>::value;
            if (det > -eps && det < eps) return false;

            float inv_det = 1.0f / det;

            VectorN<float,3> T = ray.origin - A;
            float u = T.dot(P) * inv_det;
            if (u < 0.0f || u > 1.0f) return false;

            VectorN<float,3> Q = T.cross(edge1);
            float v = ray.direction.dot(Q) * inv_det;
            if (v < 0.0f || u + v > 1.0f) return false;

            float t = edge2.dot(Q) * inv_det;
            if (t < t_min || t > t_max) return false;

            hitstruct.t = t;
            hitstruct.u = u;
            hitstruct.v = v;
            hitstruct.position = ray.at(t);
            hitstruct.normal = face_normal;
            return true;
        }


        constexpr VectorN<float,3> normal(const Hit& hit) const noexcept {
            return face_normal;
        }
   
   
    };




}