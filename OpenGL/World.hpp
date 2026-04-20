#pragma once
#include "../renderLib/Camera.hpp"
#include "../renderLib/VectorN.hpp"
#include "../renderLib/PointLight.hpp"
#include "Model.hpp"
#include "../renderLib/Camera.hpp"


namespace ES{


    
    class World{
        
        PointLight light{{0,0,0},{1,1,1}};
        std::vector<Model> instances;
        Camera camera;



        

    }
}