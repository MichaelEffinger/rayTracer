#pragma once
#include "../renderLib/Camera.hpp"
#include "../renderLib/VectorN.hpp"
#include "../renderLib/PointLight.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "../renderLib/Camera.hpp"
#include <ratio>


namespace ES{


    
    class World{
        
        bool hasCamera = false;
        bool hasLight = false;

        Texture* environmentMap = nullptr;

        
        public:
        
        RGB background;
        Camera camera;
        PointLight light;
        std::size_t height;
        std::size_t width;
        
        std::vector<Model> instances;



        World(std::size_t h, std::size_t w): light({0,0,0},{0,0,0}){
            height = h;
            width = w;
        }

        void add_instance(Model myModel){
            instances.push_back(myModel);
        }

        void add_camera(Camera myCamera){
            if(hasLight == true){
                std::cout << "camera already added";
                return;
            }
            else{
                hasCamera = true;
                camera = myCamera;
            }
        }

        void add_light(PointLight myLight){
            hasLight = true;
            light = myLight;
        }


        void draw(){
            for(auto model: instances){

                for(auto& part : model.parts){
                    if(part.material->type == RenderType::Mirror /* || part.material->type == RenderType::Glass */){
                        if(environmentMap != nullptr){
                            part.material->addTexture(TextureType::CUBEMAP, environmentMap);
                        }
                    }
                }
                model.draw(camera.projectionMatrix(),camera.viewMatrix(),light.position, camera.getPosition(),light.intensity);
            }
        }
        

    };
}