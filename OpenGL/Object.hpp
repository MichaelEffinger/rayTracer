#pragma once
#include <memory>
#include "Mesh.hpp"
#include "Material.hpp"
#include "../renderLib/AffineTransform3.hpp"


namespace ES{

    
    class Object{

    public:
        Mesh* mesh;
        Matrix<float, 4> transformation;
        Material* material;


        Object(Mesh* newMesh, Matrix<float,4> trans, Material* materialShader) noexcept{
            mesh = newMesh;
            transformation = trans;
            material = materialShader;
        }

        Object(Mesh* newMesh, AffineTransform3<float> affine,Material* materialShader){
            mesh = newMesh;
            transformation = affine.to_matrix4();
            material = materialShader;
        }

        void draw(const Matrix<float, 4>& proj, const Matrix<float, 4>& view,  
                  const PointN<float, 3>& lightPos, const PointN<float, 3>& viewPos, 
                  const RGB& lightColor, const Matrix<float, 4>& parentTransform = Matrix<float,4>::identity()) const noexcept {
            
            if (!mesh || !material || !material->shader) return;

            Matrix<float, 4> finalTransform = parentTransform * transformation;

            material->shader->use();
            material->shader->SetMat4("projMatrix", proj);
            material->shader->SetMat4("viewMatrix", view);
            material->shader->SetMat4("modelMatrix", finalTransform);
            Matrix<float, 3> normalMatrix = finalTransform.inverse().transpose().submatrix<3,3>();
            material->shader->SetMat3("normalMatrix", normalMatrix);

            material->shader->SetVec3("lightPosWorld", lightPos.x(), lightPos.y(), lightPos.z());
            material->shader->SetVec3("viewPos", viewPos.x(), viewPos.y(), viewPos.z());
            material->shader->SetVec3("lightColor", lightColor.R(), lightColor.G(), lightColor.B());

            material->bind(); 

            mesh->draw();
        }
    };
}