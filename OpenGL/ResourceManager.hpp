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
namespace ES {

    class ResourceManager {
    public:
        // Singleton Access
        static ResourceManager& getInstance() {
            static ResourceManager instance;
            return instance;
        }   

        Shader* getShader(const std::string& name, const std::string& vPath = "", const std::string& fPath = "") {
            auto it = shaders.find(name);
            if (it != shaders.end()) {
                return it->second.get();
            }
            
            if (vPath.empty() || fPath.empty()) {
                return nullptr; 
            }

            auto newShader = std::make_unique<Shader>(vPath, fPath);
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

        bool hasShader(const std::string& name) const {
            return shaders.find(name) != shaders.end();
        }

        bool hasMesh(const std::string& name) const {
            return meshes.find(name) != meshes.end();
        }

        bool hasMaterial(const std::string& name) const {
            return materials.find(name) != materials.end();
        }
    private:
        ResourceManager() {}
        ResourceManager(const ResourceManager&) = delete;
        void operator=(const ResourceManager&) = delete;

        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
        std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
        std::unordered_map<std::string, std::unique_ptr<Material>> materials;
        std::unordered_map<std::string, GLuint> textures;
    };
}