#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "Mesh.hpp"
#include "GLSL.h"
#include "../renderLib/Vertex.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include <filesystem>
namespace ES {

    class ResourceManager {
    public:
        // Singleton Access
        static ResourceManager& getInstance() {
            static ResourceManager instance;
            return instance;
        }   


        Shader* getShader(const std::string& name, const std::string& vFileName = "", const std::string& fFileName = "") {
            const std::string SHADER_PATH = "../../assets/shaders/";
            auto it = shaders.find(name);
            if (it != shaders.end()) {
                return it->second.get();
            }
            
            if (vFileName.empty() || fFileName.empty()) {
                return nullptr; 
            }

            std::filesystem::path base(SHADER_PATH);
            
            std::string fullVPath = (base / vFileName).string();
            std::string fullFPath = (base / fFileName).string();

            auto newShader = std::make_unique<Shader>(fullVPath, fullFPath);
            Shader* ptr = newShader.get();
            shaders[name] = std::move(newShader);
            
            return ptr;
        }

        Mesh* getMesh(const std::string& name, const std::vector<Vertex>& vertices = {}, const std::vector<unsigned int>& indices = {}) {
            auto it = meshes.find(name);
            if (it != meshes.end()) {
                return it->second.get();
            }

            if (vertices.empty() || indices.empty()) {
                return nullptr;
            }

            auto newMesh = std::make_unique<Mesh>(vertices, indices);
            Mesh* ptr = newMesh.get();
            meshes[name] = std::move(newMesh);
            return ptr;
        }

        Material* getMaterial(const std::string& name, Shader* shader = nullptr, const std::vector<float>& data = {}) {
            auto it = materials.find(name);
            if (it != materials.end()) {
                return it->second.get();
            }

            auto newMat = std::make_unique<Material>();
            newMat->shader = shader;
            newMat->data = data;
            
            Material* ptr = newMat.get();
            materials[name] = std::move(newMat);
            return ptr;
        }

        Texture* getTexture(const std::string& name, const std::string& path = "", TextureType type = TextureType::DIFFUSE) {
            auto it = textures.find(name);
            if (it != textures.end()) {
                return it->second.get();
            }

            // If it doesn't exist and no path was provided to load it, return null
            if (path.empty()) {
                return nullptr;
            }

            // Create it, store it, and return the raw pointer
            auto newTexture = std::make_unique<Texture>(path, type);
            Texture* ptr = newTexture.get();
            textures[name] = std::move(newTexture);
            return ptr;
        }
        
        bool hasTexture(const std::string& name) const {
            return textures.find(name) != textures.end();
        }

        bool hasShader(const std::string& name) const {
            return shaders.find(name) != shaders.end();
        }

        bool hasMesh(const std::string& name) const {
            return meshes.find(name) != meshes.end();
        }

        bool hasMaterial(const std::string& name) const {
            return materials.find(name) != materials.end();
        }
        
        ResourceManager() {}
        ResourceManager(const ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
        std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
        std::unordered_map<std::string, std::unique_ptr<Material>> materials;
        std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    private:
    };
}