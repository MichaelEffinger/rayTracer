#pragma once 
#include <vector>
#include "Object.hpp"

namespace ES {

    class Model {
    public: 
        std::vector<Object> parts;
        
        Matrix<float, 4> worldTransform = Matrix<float, 4>::identity();

        void AddPart(const Object& part) {
            parts.push_back(part);
        }

        void draw(const Matrix<float, 4>& proj, const Matrix<float, 4>& view, const PointN<float, 3>& lightPos, const PointN<float, 3>& viewPos, const RGB& lightColor) const {
            for (const auto& part : parts) {
                part.draw(proj, view,lightPos,viewPos,lightColor,worldTransform);
            }
        }
    };
}