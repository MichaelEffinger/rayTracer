#pragma once


#include "PointN.hpp"
#include <array>


namespace ES{
    struct Vertex{
        PointN<float, 3> position;
        VectorN<float,3> normal;
        std::array<float,2> uv;

        
    };
}