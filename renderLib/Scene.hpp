#pragma once
#include <vector>
#include "SceneObject.hpp"
#include "PerspectiveCamera.hpp"

namespace ES{

    class Scene{

        public:

        std::vector<SceneObject> shapes;
        std::vector<PointLight> lights;
        std::vector<PerspectiveCamera> cameras;
        RGB background;
        bool has_shadows;

        constexpr Scene(RGB background_) noexcept : background(background_){}

        Scene(SceneObject object) noexcept : shapes{object}{}


        constexpr void clear() noexcept {
            shapes.clear();
        }

        constexpr Scene& add_shape(SceneObject object){
            shapes.push_back(object);
            return *this;
        }

        constexpr Scene& add_light(PointLight light){
            lights.push_back(light);
            return *this;
        }

        constexpr Scene& add_camera(PerspectiveCamera camera){
            cameras.push_back(camera);
            return *this;
        }

        constexpr bool intersect(Ray<float> ray, float t_min, float& t_max, Hit& hitstruct, std::size_t& hit_index) const noexcept {
            bool hit_anything = false;
            float closest = t_max;

            for (std::size_t i = 0; i < shapes.size(); ++i) {
                Hit temp_hit;

                if (shape::intersect(shapes[i].obj, ray, t_min, closest, temp_hit)) {
                    hit_anything = true;
                    closest = temp_hit.t;
                    hitstruct = temp_hit;
                    hit_index = i;
                }
            }

            return hit_anything;
        }


    };


}









