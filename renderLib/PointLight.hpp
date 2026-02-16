#pragma once

#include "PointN.hpp"
#include "ColorN.hpp"


namespace ES{
    
    class PointLight{

    public:
        PointN<float,3> position;
        RGB intensity;

        constexpr PointLight( PointN<float,3> position_, RGB intensity_) noexcept : position(position_), intensity(intensity_){}

    };

}