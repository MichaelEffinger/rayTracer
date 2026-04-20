#pragma once

#include "../src/ISceneLoader.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "World.hpp"
#include "../renderLib/Sphere.hpp"
#include <iostream>
#include <unordered_map>
#include "ResourceManager.hpp"
#include "shape_converter.hpp"
#include "../renderLib/AffineTransform3.hpp"

class OpenGLSceneLoader : public ISceneLoader {
private:
    ES::World &m_world;

public:
    OpenGLSceneLoader(ES::World &sceneToPopulate)
        : m_world(sceneToPopulate), numShaders(0), numTextures(0) {}

    void reserveCameras(size_t) override {}
    void reserveLights(size_t) override {}
    void reserveShapes(size_t count) override { m_world.instances.reserve(count); }
    void reserveShaders(size_t count) override { numShaders = count; }
    void reserveTextures(size_t count) override { numTextures = count; }

    void setSceneParams(const SceneParams &) override {
        std::cout << "Setting scene params." << std::endl;
    }

    void addCamera(const std::string &name, const std::string &type,
                   const ES::Vector3<float> &pos,
                   const ES::Vector3<float> &viewDir,
                   float, float) override {

        std::cout << "Creating camera: " << name << ", type:" << type << std::endl;

        ES::Camera cam(60.0f, 1.0f, 0.1f, 1000.0f);

        ES::PointN<float,3> eye{pos.x(), pos.y(), pos.z()};
        ES::PointN<float,3> target{
            pos.x() + viewDir.x(),
            pos.y() + viewDir.y(),
            pos.z() + viewDir.z()
        };

        ES::VectorN<float,3> up{0,1,0};

        cam.setLookAt(eye, target, up);
        m_world.add_camera(cam);
    }

    void addPointLight(const ES::Vector3<float> &pos,
                       ES::RGB &intensity) override {
        std::cout << "Creating point light." << std::endl;
        m_world.add_light({pos, intensity});
    }

    void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {

        std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;

        auto &manager = ES::ResourceManager::getInstance();

        if (shaderDesc.type == "Lambertian") {

            auto shader = manager.getShader(
                "LambertianShader",
                "vertexLambertian.glsl",
                "fragmentLambertian.glsl"
            );

            std::vector<float> data = {
                shaderDesc.diffuse.data[0],
                shaderDesc.diffuse.data[1],
                shaderDesc.diffuse.data[2]
            };

            manager.getMaterial(shaderDesc.name, shader, data);
        }

        else if (shaderDesc.type == "BlinnPhong" || shaderDesc.type == "Phong") {

            auto shader = manager.getShader(
                "BlinnPhong",
                "vertexBlinnPhong.glsl",
                "fragmentBlinnPhong.glsl"
            );

            std::vector<float> data = {
                shaderDesc.diffuse.data[0],
                shaderDesc.diffuse.data[1],
                shaderDesc.diffuse.data[2],
                shaderDesc.specular.data[0],
                shaderDesc.specular.data[1],
                shaderDesc.specular.data[2],
                shaderDesc.phongExp
            };

            manager.getMaterial(shaderDesc.name, shader, data);
        }

        else {
            std::cout << "Warning: unknown shader type '"
                      << shaderDesc.type
                      << "', using fallback." << std::endl;

            auto shader = manager.getShader(
                "LambertianShader",
                "vertexLambertian.glsl",
                "fragmentLambertian.glsl"
            );

            manager.getMaterial("missing", shader, {1,0,1});
        }
    }

    void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {

        auto &manager = ES::ResourceManager::getInstance();

        std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;

        ES::Material* myMat = nullptr;

        if (manager.hasMaterial(shapeDesc.shaderNameReference)) {
            myMat = manager.getMaterial(shapeDesc.shaderNameReference);
        }
        else {
            std::cout << "Warning: material '"
                      << shapeDesc.shaderNameReference
                      << "' not found for shape '"
                      << shapeDesc.name << "'"
                      << std::endl;

            auto shader = manager.getShader(
                "LambertianShader",
                "vertexLambertian.glsl",
                "fragmentLambertian.glsl"
            );

            myMat = manager.getMaterial("missing", shader, {1,0,1});
        }

        if (shapeDesc.type == "sphere") {

            ES::Sphere mySphere({0,0,0},1);
            auto mesh = ES::to_Mesh(mySphere,5);

            ES::Object obj{
                mesh,
                ES::Matrix<float,4>::identity(),
                myMat
            };

            ES::Model model;
            model.AddPart(obj);

            model.worldTransform =
                ES::AffineTransform3<float>::from_trs(
                    shapeDesc.center,
                    ES::Quaternion<float>::identity(),
                    {shapeDesc.radius, shapeDesc.radius, shapeDesc.radius}
                ).to_matrix4();

            m_world.instances.push_back(model);
        }

        else if (shapeDesc.type == "triangle") {

            auto normal =
                (shapeDesc.v1 - shapeDesc.v0)
                .cross(shapeDesc.v2 - shapeDesc.v0)
                .normalize();

            std::vector<ES::Vertex> verts = {
                { shapeDesc.v0, normal, {0,0} },
                { shapeDesc.v1, normal, {1,0} },
                { shapeDesc.v2, normal, {0,1} }
            };

            std::vector<unsigned int> inds = {0,1,2};

            ES::Mesh* mesh =
                manager.getMesh(shapeDesc.name, verts, inds);

            ES::Object obj{
                mesh,
                ES::Matrix<float,4>::identity(),
                myMat
            };

            ES::Model model;
            model.AddPart(obj);

            m_world.instances.push_back(model);
        }

        else {
            std::cout << "Warning: unknown shape type '"
                      << shapeDesc.type << "'" << std::endl;
        }
    }

    void addTexture(const std::string &type,
                    const std::string &name,
                    const std::string &sourceFile) override {
        std::cout << "Creating texture: type=" << type
                  << ", name=" << name
                  << ", sourceFile=" << sourceFile << std::endl;
    }

public:
    int numShaders;
    int numTextures;
};