#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Shader.hpp"
#include "../renderLib/VectorN.hpp"
#include "../renderLib/ColorN.hpp"
#include "Texture.hpp"

namespace ES{

    enum class RenderType { Standard, Mirror, Glass };

    class Material {
    public:

        Shader* shader = nullptr;
        std::vector<float> data;
        std::unordered_map<TextureType, Texture*> textures;
        RenderType type;
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
                
                glActiveTexture(GL_TEXTURE0 + unit);

                if (type == TextureType::CUBEMAP) {
                    glBindTexture(GL_TEXTURE_CUBE_MAP, tex->getID());
                } 
                else {
                    glBindTexture(GL_TEXTURE_2D, tex->getID());
                }
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
                case TextureType::CUBEMAP:  return "u_skybox";
                default:                    return "texture_unknown";
            }
        }
    };
}