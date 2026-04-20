#pragma once

#include "ResourceManager.hpp"
#include "../renderLib/Sphere.hpp"
#include "../renderLib/ES_math.hpp"


namespace ES{

constexpr Mesh* to_Mesh(Sphere sphere, int subdivisions) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const int sectors = subdivisions * 2; 
    const int stacks = subdivisions; 
    
    static constexpr float PI = ES::math::pi<float>;

    float x, y, z, xy;                          
    float lengthInv = 1.0f / sphere.radius;

    float sectorStep = 2.0f * PI / sectors;
    float stackStep = PI / stacks;

    for(int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2.0f - i * stackStep;      
        xy = sphere.radius * std::cos(stackAngle);
        z = sphere.radius * std::sin(stackAngle);  

        for(int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep; 

            x = xy * std::cos(sectorAngle);   
            y = xy * std::sin(sectorAngle); 
        
            Vertex v;
            // Position
            v.position.x() = x + sphere.origin.x();
            v.position.y() = y + sphere.origin.y();
            v.position.z() = z + sphere.origin.z();

            // Normal
            v.normal.x() = x * lengthInv;
            v.normal.y() = y * lengthInv;
            v.normal.z() = z * lengthInv;

            // UVs - Fixed: index 0 and 1
            v.uv[0] = (float)j / sectors;
            v.uv[1] = (float)i / stacks;

            vertices.push_back(v);
        }
    }

    // Index Generation
    for(int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1); 
        int k2 = k1 + sectors + 1; 

        for(int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if(i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    // Dynamic string ID based on radius and quality
    std::string meshID = "Sphere_r" + std::to_string(sphere.radius) + "_s" + std::to_string(subdivisions);
    
    return ResourceManager::getInstance().getMesh(meshID, vertices, indices);
}
}