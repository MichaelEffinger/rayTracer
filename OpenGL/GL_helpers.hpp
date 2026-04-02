#pragma once
#include <vector>
#include "../renderLib/Triangle.hpp"
#include "../renderLib/ColorN.hpp"
#define IDK 4



namespace ES{

    // greyScale
    inline std::vector<float > to_buffer(ES::Triangle tri,RGB color1, RGB color2, RGB color3){
        return{
            tri.A.x(), tri.A.y(), tri.A.z(),
            color1.R(),color1.G(),color1.B(),
            tri.B.x(), tri.B.y(), tri.B.z(),
            color2.R(),color2.G(),color2.B(),
            tri.C.x(), tri.C.y(), tri.C.z(),
            color3.R(),color3.G(),color3.B(),
        };
    }

}