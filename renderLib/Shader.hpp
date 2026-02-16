#pragma once

#include <variant>
#include <random>
#include "ColorN.hpp"
#include "Ray.hpp"
#include "VectorN.hpp"
#include "PointLight.hpp"
#include "Hit.hpp"
namespace ES{
    

    
    class ConstantShader{
        public:

        RGB color;


        ConstantShader(RGB newColor){
            color = newColor;
        }

        constexpr RGB operator()(const Hit& hit,const Ray<float>& ray, const std::vector<PointLight>& lights) const noexcept{
            return color;
        }
    };

    class RandomShader{
    
        public:
        mutable std::mt19937 rng;
        mutable std::uniform_real_distribution<float> dist;


        RandomShader(){ };

        RGB operator()(const Hit& hit,const Ray<float>& ray,const std::vector<PointLight>& lights) const noexcept{
            return { dist(rng), dist(rng), dist(rng) };
        }
    };

    class LambertianShader{
    public:
        RGB color;
        float intensity;

        LambertianShader(RGB newColor, float intensity_) : color(newColor), intensity(intensity_) {}

        constexpr RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights) const noexcept {
            RGB finalColor{0,0,0};

            for(const auto& light : lights){
                VectorN<float,3> light_direction = (light.position - hit.position).normalize();
                float factor = std::max(0.0f, hit.normal.dot(light_direction));
                finalColor += (color * intensity) * light.intensity * factor;
            }
            return finalColor;
            
        }
    };

    class RandomLambertianShader{
    public:
        RGB color;
        float intensity;
        mutable std::mt19937 rng;
        mutable std::uniform_real_distribution<float> dist;

        RGB operator()(const Hit& hit, const Ray<float>& ray, const std::vector<PointLight>& lights) const noexcept {
            RGB finalColor{0,0,0};

            for(const auto& light : lights){
                VectorN<float,3> light_direction = (light.position - hit.position).normalize();
                float factor = std::max(0.0f, hit.normal.dot(light_direction));
                finalColor += (RGB{dist(rng),dist(rng),dist(rng)} * intensity) * light.intensity * factor;
            }
            return finalColor;
        }

    };

    class NormalShader{
        public:
        
        NormalShader(){};

        constexpr RGB operator()(const Hit& hit,const Ray<float>& ray,const std::vector<PointLight>& lights)const noexcept{
            return RGB{0.5f * (hit.normal.x() + 1.f), 0.5f * (hit.normal.y() + 1.f), 0.5f * (hit.normal.z() + 1.f)};            
        }

    };

    class BlinnPhongShader{
        public:
        RGB color;
        float shininess;
        float intensity;
        RGB specular_color;
        
        BlinnPhongShader(RGB color_, float shininess_,float intensity_, RGB specular_color_ = {1,1,1}) noexcept : 
           color(color_),shininess(shininess_), intensity(intensity_),specular_color(specular_color_){}

        RGB operator()(const Hit& hit,const Ray<float>& ray,const std::vector<PointLight>& lights) const noexcept{ 
            RGB finalColor{0,0,0};

            for(const auto& light : lights){
                VectorN<float,3> L = (light.position - hit.position).normalize();
                VectorN<float,3> V = (ray.origin - hit.position).normalize(); 
                VectorN<float,3> H = (L + V).normalize();


                float diffuse_factor = std::max(0.0f, hit.normal.dot(L));
                RGB diffuse = (color * intensity) * light.intensity * diffuse_factor;

                float shine_factor = std::max(0.0f, hit.normal.dot(H));
                float specular_intensity = std::pow(shine_factor, shininess);
                RGB specular = specular_color * light.intensity * specular_intensity;

                finalColor += diffuse + specular;
            }


            return finalColor;
            
        }
    };

    
    
    using Shader = std::variant<ConstantShader,RandomShader,NormalShader,LambertianShader,BlinnPhongShader, RandomLambertianShader>;



    namespace shader{
        constexpr RGB shade(const Shader& shader_, const Hit& hit, const Ray<float>& ray,const std::vector<PointLight>& lights) noexcept{
            return std::visit([&](auto&& sh) -> RGB { return sh(hit, ray,lights); }, shader_);
        }
    }







}