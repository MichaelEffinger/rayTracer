#include <cmath>
#include <iostream>
#include <random>
#include "ES_color_util.hpp"
#include "ColorN.hpp"
#include "FrameBuffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Ray.hpp"
#include "Shader.hpp"
#include "Shape.hpp"
#include "SceneObject.hpp"

using namespace ES;

int main(int argc, char *argv[]) {
    
    sivelab::GraphicsArgs args;
    std::random_device rd;
    std::mt19937_64 randy(rd());
    std::uniform_real_distribution<float> dist;
   
    args.process(argc, argv);
    FrameBuffer fb;
    fb.set_height_width(800, 800);

    ES::PointN<float,3> cam_pos{0,0,0};
    ES::VectorN<float,3> cam_dir{0,0,-1};
    float focal = 1.f;
    float image_plane_width = .5f;
    std::size_t width = 800;
    std::size_t height = 800;
    ES::VectorN<float,3> light_dir = {-1, -1, -1};
    light_dir.normalize_in_place();



    float TMIN = 0;
    float TMAX = INFINITY;

    ES::PerspectiveCamera p(cam_pos, cam_dir, focal, image_plane_width, width, height);
    
    fb.gradiant_up({1,1,1},{0,0,0});

    std::vector<SceneObject> scene;
    RGB ambient  = {0.15f, 0.12f, 0.05f};
    RGB diffuse  = {0.55f, 0.44f, 0.15f};
    RGB specular = {0.8f, 0.7f, 0.2f};
    float shininess = 48.0f;
    float intensity = 1.0f;


    BlinnPhongShader goldShader(ambient, diffuse, specular, shininess, intensity);

    
    scene.push_back({Sphere({ 4.f,  0.f, -20.f}, 1.75f),LambertianShader({1,0,0},1)});
    scene.push_back({Sphere({ 2.f,  3.464f, -20.f}, 1.75f), LambertianShader({0,1,0},1)});
    scene.push_back({Sphere({-2.f,  3.464f, -20.f}, 1.75f), LambertianShader({0,0,1},1)});
    scene.push_back({Sphere({-4.f,  0.f, -20.f}, 1.75f),LambertianShader({1,1,0},1)});
    scene.push_back({Sphere({-2.f, -3.464f, -20.f}, 1.75f), LambertianShader({0,1,1},1)});
    scene.push_back({Sphere({ 2.f, -3.464f, -20.f}, 1.75f), LambertianShader({1,0,1},1)});
    scene.push_back({Sphere({0.f, 0.f, -20.f}, 1.75f), RandomLambertianShader{1} });
    

    for (int x=0; x <800; ++x) {
        for (int y=0; y <800; ++y) {
            Ray<float> r = p.generateRay(y, x);
            
            float t_max = TMAX;
            Hit hit;
            bool hit_anything = false;
            std::size_t hit_index = 0;

            for(int i=0; i<scene.size(); i++){
                Hit temp_hit;
                if(shape::intersect(scene[i].obj,r,TMIN, t_max, temp_hit)){
                    hit_anything = true;
                    hit = temp_hit;
                    hit_index = i;
                }
            }

            if(hit_anything){
                fb(y,x) = shader::shade(scene[hit_index].shade,hit.position,hit.normal,r,light_dir);
            }
        }
    }

    
    ES::export_as_PNG(fb, "defaultCamRayColors.png");
    
}