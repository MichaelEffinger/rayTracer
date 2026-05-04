#pragma once
#include "ResourceManager.hpp"
#include "Material.hpp"
#include "../renderLib/ColorN.hpp"

namespace ES{

    inline Material make_blinn_phong(RGB color, float shininess, RGB specular_color) noexcept{
        Material newMat;
        newMat.shader = ResourceManager::getInstance().getShader("BlinnPhong","vertexShader.glsl","BlinnPhong.glsl");
        newMat.data = {color.R(),color.G(),color.B(),specular_color.R(),specular_color.G(),specular_color.B(),shininess};
        return newMat;
    } 

    inline Material make_lambertian(RGB color) noexcept{
        Material newMat;
        newMat.shader = ResourceManager::getInstance().getShader("LambertianShader", "vertexShader.glsl", "Lambertian.glsl");
        newMat.data ={color.R(),color.G(),color.B()};
        return newMat;
    }

    inline Material make_schlick(RGB C_lamba, float roughness, float isotrophy) noexcept{
        Material newMat;    
        newMat.shader = ResourceManager::getInstance().getShader("SchlickShader", "vertexShader.glsl", "Schlick.glsl");
        newMat.data = {C_lamba.R(), C_lamba.G(),C_lamba.B(),roughness,isotrophy};
        return newMat;
    }




}