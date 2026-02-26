#pragma once
#include <vector>
#include "SceneObject.hpp"

namespace ES{

    class ShapeList{

        public:

        std::vector<SceneObject> data_;
        RGB background;

        constexpr ShapeList(RGB background_) noexcept : background(background_){}

        ShapeList(SceneObject object) noexcept : data_{object}{}


        constexpr void clear() noexcept {
            data_.clear();
        }

        constexpr ShapeList& add(SceneObject object){
            data_.push_back(object);
            return *this;
        }

        constexpr bool intersect(Ray<float> ray, float t_min, float& t_max, Hit& hitstruct, std::size_t& hit_index) const noexcept {
            bool hit_anything = false;
            float closest = t_max;

            for (std::size_t i = 0; i < data_.size(); ++i) {
                Hit temp_hit;

                if (shape::intersect(data_[i].obj, ray, t_min, closest, temp_hit)) {
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









