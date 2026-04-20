#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Shader.hpp"
#include "../renderLib/VectorN.hpp"
#include "../renderLib/ColorN.hpp"


namespace ES{
    class Material {
    public:
        Shader* shader = nullptr;
        std::vector<float> data;

        // for very generic obj style shaders here is the ordering 
        // Order: Diffuse[0-2], Specular[3-5], Shininess[6], Alpha[7]

    };

}