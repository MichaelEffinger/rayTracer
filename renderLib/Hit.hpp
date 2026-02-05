#pragma once


#include "VectorN.hpp"
#include "PointN.hpp"

namespace ES{


    struct Hit{

        float t;
        PointN<float,3> position;
        VectorN<float,3> normal;


    };

    

}