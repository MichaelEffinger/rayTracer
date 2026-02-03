#include <cmath>
#include <iostream>

#include "ES_color_util.hpp"
#include "ColorN.hpp"
#include "FrameBuffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include "SceneObject.hpp"
using namespace ES;

int main(int argc, char *argv[]) {
    
    sivelab::GraphicsArgs args;
    args.process(argc, argv);
    FrameBuffer fb;
    fb.set_height_width(800, 800);

    ES::PointN<float,3> cam_pos{0,0,0};
    ES::VectorN<float,3> cam_dir{0,0,-1};
    float focal = 1.f;
    float image_plane_width = .5f;
    std::size_t width = 800;
    std::size_t height = 800;

    ES::PerspectiveCamera p(cam_pos, cam_dir, focal, image_plane_width, width, height);
    
    fb.gradiant_up({1,1,1},{0,0,0});

    std::vector<SceneObject> scene;
    
    scene.push_back({Sphere({ 4.f,  0.f, -20.f}, 1.75f),ConstantShader({1,0,0})});
    scene.push_back({Sphere({ 2.f,  3.464f, -20.f}, 1.75f), ConstantShader({0,1,0})});
    scene.push_back({Sphere({-2.f,  3.464f, -20.f}, 1.75f), ConstantShader({0,0,1})});
    scene.push_back({Sphere({-4.f,  0.f, -20.f}, 1.75f),ConstantShader({1,1,0})});
    scene.push_back({Sphere({-2.f, -3.464f, -20.f}, 1.75f), ConstantShader({0,1,1})});
    // scene.push_back({Sphere({ 2.f, -3.464f, -20.f}, 1.75f), ConstantShader({1,0,1})});

    scene.push_back({Sphere({0.f, 0.f, -20.f}, 1.75f), ConstantShader({1,1,1})});

    

    for (int x=0; x <800; ++x) {
        for (int y=0; y <800; ++y) {
            Ray<float> r = p.generateRay(y, x);
            
            float closest = INFINITY;
            bool hit_anything = false;
            std::size_t hit_index = 0;

            for(int i=0; i<scene.size(); i++){
                float t_hit;
                if(shape::intersect(scene[i].obj,r,t_hit)&& t_hit <closest){
                    closest = t_hit;
                    hit_index = i;
                    hit_anything = true;
                }
            }

            if(hit_anything){
                PointN<float,3> hit_position = r.origin + closest* r.direction;
                VectorN<float,3> normal = shape::normal(scene[hit_index].obj,hit_position);
                fb(y,x) = shader::shade(scene[hit_index].shade,hit_position,normal,r);
            }
        }
    }

    
    ES::export_as_PNG(fb, "defaultCamRayColors.png");
    
}