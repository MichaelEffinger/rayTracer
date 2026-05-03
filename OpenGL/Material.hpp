#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Shader.hpp"
#include "../renderLib/VectorN.hpp"
#include "../renderLib/ColorN.hpp"
#include "Texture.hpp"

namespace ES{
    class Material {
    public:

        Shader* shader = nullptr;
        std::vector<float> data;
        std::unordered_map<TextureType, Texture*> textures;

        void addTexture(TextureType type, Texture* tex) {
            textures[type] = tex;
        }

        void bind() {
            if (!shader) return;
            auto it = textures.find(TextureType::DIFFUSE);
            bool hasValidTexture = (it != textures.end() && it->second != nullptr && it->second->getID() != 0);

            shader->SetBool("u_useTexture", hasValidTexture);

            unsigned int unit = 0;
            for (auto const& [type, tex] : textures) {
                if (!tex || tex->getID() == 0) continue; 
                
                tex->bind(unit);
                shader->SetInt(getTextureName(type), unit);
                unit++;
            }

            if (data.size() >= 8) {
                shader->SetFloatArray("u_materialData", data);
            }
        }

    private:

        const char* getTextureName(TextureType type) {
            switch(type) {
                case TextureType::DIFFUSE:  return "texture_diffuse";
                case TextureType::SPECULAR: return "texture_specular";
                case TextureType::NORMAL:   return "texture_normal";
                default:                    return "texture_unknown";
            }
        }
    };
}