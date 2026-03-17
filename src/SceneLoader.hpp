#pragma once

#include "ISceneLoader.hpp"
#include "../renderLib/Scene.hpp"
#include "../renderLib/Sphere.hpp"
#include <iostream>

class SceneLoader : public ISceneLoader {
private:
  ES::Scene &m_targetScene; // Reference to the external scene
  std::unordered_map<std::string, ES::Shader> shaderMap;

public:
  // The caller provides the scene to be filled
  SceneLoader(ES::Scene &sceneToPopulate)
      : m_targetScene(sceneToPopulate), numShaders(0), numTextures(0) {}

  void reserveCameras(size_t count) override {
    // makes sure the vector has enough space
    m_targetScene.cameras.reserve(count);
  }
  void reserveLights(size_t count) override {
    m_targetScene.lights.reserve(count);
  }
  void reserveShapes(size_t count) override {
    m_targetScene.shapes.reserve(count);
  }
  void reserveShaders(size_t count) override { numShaders = count; }
  void reserveTextures(size_t count) override { numTextures = count; }

  void setSceneParams(const SceneParams &sparams) override {
    std::cout << "Setting scene params." << std::endl;
  }

  void addCamera(const std::string &name, const std::string &type,
                 const ES::Vector3<float> &pos, const ES::Vector3<float> &viewDir,
                 float focalLength, float imagePlaneWidth) override {
        std::cout << "Creating camera: " << name << ", type:" << type << std::endl;
        m_targetScene.add_camera({pos, viewDir,focalLength, imagePlaneWidth, m_targetScene.width, m_targetScene.height});
  }
  void addPointLight(const ES::Vector3<float> &pos,
                    ES::RGB &intensity) override {
    std::cout << "Creating point light." << std::endl;
    m_targetScene.add_light({pos,intensity});
  }
  /*
  void addAreaLight(const ES::Vector3<float> &pos,
                    const ES::RGB &color,
                    const ES::Vector3<float> &normal, float width,
                    float length) override {
    std::cout << "Creating area light." << std::endl;
  }
  */



void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {
    std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;

    if (shaderDesc.type == "Lambertian") {
        shaderMap[shaderDesc.name] = ES::LambertianShader({shaderDesc.diffuse.data[0],shaderDesc.diffuse.data[1],shaderDesc.diffuse.data[2]},1.0);
    }
    else if (shaderDesc.type == "white_chalk") {
        shaderMap[shaderDesc.name] = ES::LambertianShader({1,1,1},1.0);
    }
    else if (shaderDesc.type == "red_chalk") {
        shaderMap[shaderDesc.name] = ES::LambertianShader({1,0,0},1.0);
    }
    else if (shaderDesc.type == "yellow_chalk") {
        shaderMap[shaderDesc.name] = ES::LambertianShader({1,1,0},1.0);
    }
    else if (shaderDesc.type == "blue_chalk") {
        shaderMap[shaderDesc.name] = ES::LambertianShader({0,0,1},1.0);
    }
    else if (shaderDesc.type == "BlinnPhong" || shaderDesc.type == "Phong") {
        shaderMap[shaderDesc.name] = ES::BlinnPhongShader({shaderDesc.diffuse.data[0],shaderDesc.diffuse.data[1], shaderDesc.diffuse.data[2]}, shaderDesc.phongExp,1.0f,{shaderDesc.specular.data[0],shaderDesc.specular.data[1],shaderDesc.specular.data[2]});
    }
    else if (shaderDesc.type == "Mirror") {
        shaderMap[shaderDesc.name] = ES::MirrorShader();
    }
    else if (shaderDesc.type == "Diffuse") {
        shaderMap[shaderDesc.name] = ES::DiffuseShader({shaderDesc.diffuse.data[0],shaderDesc.diffuse.data[1],shaderDesc.diffuse.data[2]});
    }
    else if (shaderDesc.type == "Normal") {
        shaderMap[shaderDesc.name] = ES::NormalShader();
    }
    else if (shaderDesc.type == "Constant") {
        shaderMap[shaderDesc.name] = ES::ConstantShader({shaderDesc.diffuse.data[0],shaderDesc.diffuse.data[1],shaderDesc.diffuse.data[2]});
    }
    else {
        std::cout << "Warning: unknown shader type '" << shaderDesc.type << "', using magenta fallback." << std::endl;
        shaderMap[shaderDesc.name] = ES::ConstantShader(ES::RGB{1, 0, 1});
    }

}

void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
    std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;

    // look up the shader, fall back to magenta if not found
    ES::Shader shader = ES::ConstantShader(ES::RGB{1, 0, 1});
    if (shaderMap.count(shapeDesc.shaderNameReference)) {
        shader = shaderMap[shapeDesc.shaderNameReference];
    } else {
        std::cout << "Warning: shader '" << shapeDesc.shaderNameReference << "' not found for shape '" << shapeDesc.name << "'" << std::endl;
    }

    if (shapeDesc.type == "sphere") {
        ES::Sphere sphere(shapeDesc.center, shapeDesc.radius);
        m_targetScene.shapes.push_back(ES::SceneObject(sphere, shader));
    }
    else if (shapeDesc.type == "triangle") {
        ES::Triangle triangle(shapeDesc.v0, shapeDesc.v1, shapeDesc.v2);
        m_targetScene.shapes.push_back(ES::SceneObject(triangle, shader));
    }
    else {
        std::cout << "Warning: unknown shape type '" << shapeDesc.type << "'" << std::endl;
    }
}


  void addTexture(const std::string &type, const std::string &name,
                  const std::string &sourceFile) override {
    std::cout << "Creating texture: type=" << type << ", name=" << name
              << ", sourceFile=" << sourceFile << std::endl;
  }

public:
  int numShaders;
  int numTextures;
};
