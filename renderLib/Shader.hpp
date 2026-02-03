#pragma once

#include <variant>
#include "ColorN.hpp"
#include "Ray.hpp"
#include "VectorN.hpp"

namespace ES{
    

    
    class ConstantShader{
        public:

        RGB color;


        ConstantShader(RGB newColor){
            color = newColor;
        }

        constexpr RGB operator()(const PointN<float,3>& hit_point,const VectorN<float,3>& normal,const Ray<float>& ray) const noexcept{
            return color;
        }
    };

    
    
    using Shader = std::variant<ConstantShader>;



    namespace shader{
        constexpr RGB shade(const Shader& shader_, const PointN<float,3>& hit_point, const VectorN<float,3>& normal, const Ray<float>& ray) noexcept{
            return std::visit([&](auto&& sh) -> RGB { return sh(hit_point, normal, ray); }, shader_);
        }
    }







}