#pragma once
#include <cstddef>
#include <cmath>
#include <algorithm>
#include <cassert>
#include "VectorN.hpp"
#include "PointN.hpp"
#include "Ray.hpp"

namespace ES {

class PerspectiveCamera {
private:
    PointN<float,3> pos; 
    VectorN<float,3> U,V,W;

    float l, r, b, t;
    std::size_t nx, ny;
    float focal_length;

public:

    constexpr PerspectiveCamera() : pos{0.f, 0.f, 0.f}, U{1.f, 0.f, 0.f}, V{0.f, 1.f, 0.f}, W{0.f, 0.f, -1.f}, l(0.f), r(0.f), b(0.f), t(0.f), nx(0), ny(0), focal_length(0.f){}

    constexpr PerspectiveCamera(PointN<float,3> origin , VectorN<float,3> viewdir , float focalLength , float imagePlaneWidth , std::size_t width, std::size_t height): pos(origin), nx(width), ny(height), focal_length(focalLength){
       
        W = (-viewdir).normalize();
        U = W.cross(VectorN<float,3>{0,1,0}.normalize());
        V = U.cross(W);


        float aspect_ratio = float(width) / float(height);
        float half_width  = imagePlaneWidth * 0.5f;
        float half_height = half_width / aspect_ratio;

        l = -half_width;
        r =  half_width;
        b = -half_height;
        t =  half_height;
    }


    Ray<float> generateRay(int i, int j) noexcept {
        float u = r + (l - r) * (j + 0.5f) / float(nx); 
        float v = b + (t - b) * (i + 0.5f) / float(ny);
        return Ray<float>{pos,(-W*focal_length + U*u + V*v)};
    }
};

} 
