#pragma once

#include <variant>
#include <random>
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

        constexpr RGB operator()(const PointN<float,3>& hit_point,const VectorN<float,3>& normal,const Ray<float>& ray, const VectorN<float,3>& light_direction) const noexcept{
            return color;
        }
    };

    class RandomShader{
    
        public:
        mutable std::mt19937 rng;
        mutable std::uniform_real_distribution<float> dist;


        RandomShader(){ };

        constexpr RGB operator()(const PointN<float,3>& hit_point,const VectorN<float,3>& normal,const Ray<float>& ray,const VectorN<float,3>& light_direction) const noexcept{
            return { dist(rng), dist(rng), dist(rng) };
        }
    };

    class LambertianShader{
    public:
        RGB color;
        float intensity;

        LambertianShader(RGB newColor, float intensity_) : color(newColor), intensity(intensity_) {}

        constexpr RGB operator()(const PointN<float,3>& hit_point, const VectorN<float,3>& normal, const Ray<float>& ray, const VectorN<float,3>& light_direction) const noexcept {
            VectorN<float,3> L = -light_direction;
            float NdotL = std::max(0.f, normal.dot(L));
            RGB temp = color * intensity * NdotL;
            return temp.clamp(0,1);
        }
    };

    class RandomLambertianShader{
    public:
        RGB color;
        float intensity;
        mutable std::mt19937 rng;
        mutable std::uniform_real_distribution<float> dist;

        RandomLambertianShader(float intensity_) : intensity(intensity_) {}

        constexpr RGB operator()(const PointN<float,3>& hit_point, const VectorN<float,3>& normal, const Ray<float>& ray, const VectorN<float,3>& light_direction) const noexcept {
            VectorN<float,3> L = -light_direction;
            float NdotL = std::max(0.f, normal.dot(L));
            RGB temp = RGB{dist(rng),dist(rng),dist(rng)} * intensity * NdotL;
            return temp.clamp(0,1);
        }
    };




    class NormalShader{
        public:
        
        NormalShader(){};

        constexpr RGB operator()(const PointN<float,3>& hit_point,const VectorN<float,3>& normal,const Ray<float>& ray,const VectorN<float,3>& light_direction)const noexcept{
            return RGB{0.5f * (normal.x() + 1.f), 0.5f * (normal.y() + 1.f), 0.5f * (normal.z() + 1.f)};
        }



    };

    class BlinnPhongShader{
        public:
        RGB ambient;
        RGB diffuse;
        RGB specular;
        float shininess;
        float intensity;
        
        BlinnPhongShader(RGB ambient_, RGB diffuse_, RGB specular_, float shininess_,float intensity_){
            ambient = ambient_;
            diffuse = diffuse_;
            specular = specular_;
            shininess_ = shininess_;
            intensity = intensity_;
        }

        constexpr RGB operator()(const PointN<float,3>& hit_point,const VectorN<float,3>& normal,const Ray<float>& ray,const VectorN<float,3>& light_direction) const noexcept{
                
            VectorN<float,3> N = normal.normalize();
            VectorN<float,3> L = -light_direction.normalize();
            VectorN<float,3> V = (-ray.direction).normalize();
            VectorN<float,3> H = (L + V).normalize(); 

            float NdotL = std::max(0.f, N.dot(L));
            float NdotH = std::max(0.f, N.dot(H));

            RGB color = ambient + diffuse * (NdotL * intensity) + specular * std::pow(NdotH, shininess) * intensity;

            return color.clamp(0.f, 1.f);
        }
    };

    
    
    using Shader = std::variant<ConstantShader,RandomShader,NormalShader,LambertianShader,BlinnPhongShader, RandomLambertianShader>;



    namespace shader{
        constexpr RGB shade(const Shader& shader_, const PointN<float,3>& hit_point, const VectorN<float,3>& normal, const Ray<float>& ray,const VectorN<float,3>& light_direction) noexcept{
            return std::visit([&](auto&& sh) -> RGB { return sh(hit_point, normal, ray,light_direction); }, shader_);
        }
    }







}