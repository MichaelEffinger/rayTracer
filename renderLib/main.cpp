#include <cmath>
#include <iostream>

#include "ES_color_util.hpp"
#include "ColorN.hpp"
#include "FrameBuffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Ray.hpp"
#include "Shader.hpp"
#include "Shape.hpp"
#include "SceneObject.hpp"
#include "ShapeList.hpp"
#include "PointLight.hpp"
using namespace ES;

int main(int argc, char *argv[]) {
    
    sivelab::GraphicsArgs args;
    args.process(argc, argv);
    FrameBuffer fb;
    fb.set_height_width(1000, 1000);

    ES::PointN<float,3> cam_pos{0,0,0};
    ES::VectorN<float,3> cam_dir{0,0,-1};
    float focal = 1.f;
    float image_plane_width = .5f;
    std::size_t width = 1000;
    std::size_t height = 1000;
    float TMAX = INFINITY;
    float TMIN = 0;
    PointLight light{{5,0,-17},{1,1,1}};

    std::vector<PointLight> lights{light};
    //lights.push_back({{-5,0,-17},{1,1,1}});

    int samples_per_pixel = 10; 

    ES::PerspectiveCamera p(cam_pos, cam_dir, focal, image_plane_width, width, height);
    
    fb.gradiant_up({1,1,1},{0,0,0});

    ShapeList scene;

    
    scene.add({Sphere({ 4.f,  0.f, -20.f}, 1.75f),LambertianShader({1,0,0},1)});
    scene.add({Sphere({ 2.f,  3.464f, -20.f}, 1.75f), LambertianShader({0,1,0},1)});
    scene.add({Sphere({-2.f,  3.464f, -20.f}, 1.75f), LambertianShader({0,0,1},1)});
    scene.add({Sphere({-4.f,  0.f, -20.f}, 1.75f),LambertianShader({1,1,0},1)});
    scene.add({Sphere({-2.f, -3.464f, -20.f}, 1.75f), LambertianShader({0,1,1},1)});
    scene.add({Sphere({ 2.f, -3.464f, -20.f}, 1.75f), BlinnPhongShader({1,0,1},25,1,{1,1,1})});

    
    scene.add({Sphere({0.f, 0.f, -20.f}, 1.75f), LambertianShader({1,1,1},1)});

    scene.add({Triangle({ 4.f,  0.f, -20.f},{-2.f,  3.464f, -20.f},{-2.f, -3.464f, -20.f}),LambertianShader({.31,0,.31},1)});
    scene.add({Triangle({ 2.f,  3.464f, -20.f},{-4.f,  0.f, -20.f},{2.f, -3.464f, -20.f}),LambertianShader({1,.34,0},1)});
    

    for (int x=0; x <1000; ++x) {
            for (int y=0; y <1000; ++y) {
                Ray<float> r = p.generateRay(y, x);
                
                float t_max = TMAX;
                Hit hit;
                bool hit_anything = false;
                std::size_t hit_index = 0;         
                hit_anything = scene.intersect(r,TMIN, t_max,hit,hit_index);

                if(hit_anything){
                    fb(y,x) = shader::shade(scene.data_[hit_index].shade,hit, r,lights);
                }
            }
    }

    
    ES::export_as_PNG(fb, "defaultCamRayColors.png");
    
}