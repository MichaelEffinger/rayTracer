#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../renderLib/Triangle.hpp"
#include "../renderLib/ColorN.hpp"
#include "Model.hpp"
#include "OBJ_loader.hpp"
#include "ResourceManager.hpp"



namespace ES{

    // greyScale
    inline std::vector<float > to_buffer(ES::Triangle tri,RGB color1, RGB color2, RGB color3){
        return{
            tri.A.x(), tri.A.y(), tri.A.z(),
            color1.R(),color1.G(),color1.B(),
            tri.B.x(), tri.B.y(), tri.B.z(),
            color2.R(),color2.G(),color2.B(),
            tri.C.x(), tri.C.y(), tri.C.z(),
            color3.R(),color3.G(),color3.B()
        };
    }


    inline std::vector<float> to_buffer(ES::Triangle tri){
        return{
            tri.A.x(), tri.A.y(), tri.A.z(),
            tri.NA.x(),tri.NA.y(),tri.NA.z(),
            tri.B.x(), tri.B.y(), tri.B.z(),
            tri.NB.x(),tri.NB.y(),tri.NB.z(),
            tri.C.x(), tri.C.y(), tri.C.z(),
            tri.NC.x(), tri.NC.y(), tri.NC.z()
        };
    }

    inline Model load_model_from_obj(const std::string& path, Shader* shaderPtr) {

        OBJModel rawData = OBJLoader::load(path);
        Model finalModel;
        
        ResourceManager& res = ResourceManager::getInstance();

        for (size_t i = 0; i < rawData.model.size(); ++i) {
            const auto& part = rawData.model[i];

            std::string meshName = path + "_" + std::to_string(i);
            Mesh* meshPtr = res.getMesh(meshName, part.vertices, part.indices);

            std::string matName = path + "_mat_" + std::to_string(i);
            Material* partMaterial = res.getMaterial(matName, shaderPtr, part.material_data);

            Object objPart(meshPtr, Matrix<float, 4>::identity(), partMaterial);
            finalModel.AddPart(objPart);
        }

        return finalModel;
    }

}