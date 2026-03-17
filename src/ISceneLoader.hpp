#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <string>
#include "../renderLib/VectorN.hpp"
#include "../renderLib/ColorN.hpp"

class ISceneLoader {
public:
  virtual ~ISceneLoader() = default;

  virtual void reserveCameras(size_t count) = 0;
  virtual void reserveLights(size_t count) = 0;
  virtual void reserveShapes(size_t count) = 0;
  virtual void reserveShaders(size_t count) = 0;
  virtual void reserveTextures(size_t count) = 0;



  struct SceneParams {
    ES::Vector3<float> bgColor = {0.0f, 0.0f, 0.0f};
    std::string envMapFile = "";
  };

  virtual void setSceneParams(const SceneParams &sparams) = 0;

  struct ShaderProperty {
    ES::Vector3<float> data = {0.0f, 0.0f, 0.0f}; // Default to black
    std::string textureName = "";
    bool hasTexture = false;
  };

  struct ShaderDesc {
    std::string name;
    std::string type;
    ShaderProperty diffuse;
    ShaderProperty specular;
    ShaderProperty normal;
    ShaderProperty emission;
    float phongExp = 1.0f;
    float mirrorCoef = 0.0f;
    float roughness = 0.0f;
    ES::Vector3<float> attenuationCoef = {1.0f, 1.0f, 1.0f};
    float refractiveIndex = 1.0f;
  };

  virtual void addCamera(const std::string &name, const std::string &type,
                         const ES::Vector3<float> &pos, const ES::Vector3<float> &viewDir, float focalLength,
                         float imagePlaneWidth) = 0;
  virtual void addPointLight(const ES::Vector3<float> &pos, ES::RGB &intensity) = 0;
  /*
  virtual void addAreaLight(const ES::Vector3<float> &pos,
                            const ES::RGB &color,
                            const ES::Vector3<float> &normal, float width,
                            float length) = 0;



  */

  virtual void addShader(const ISceneLoader::ShaderDesc &shaderDesc) = 0;

  struct ShapeDesc {
    std::string type;
    std::string name;

    bool hasXForm = false;
    glm::mat4 localXForm;

    std::string shaderNameReference;

    ES::Vector3<float> center;   // for sphere
    float radius; // for sphere

    ES::Vector3<float> v0, v1, v2; // triangle vertices

    ES::Vector3<float> minPt, maxPt; // box minPt, maxPt

    std::string meshFilePath;

    std::string instanceId;
  };

  virtual void addShape(const ShapeDesc &desc) = 0;

  virtual void addTexture(const std::string &type, const std::string &name,
                          const std::string &sourceFile) = 0;
};
