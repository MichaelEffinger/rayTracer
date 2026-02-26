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
#include "Renderer.hpp"
using namespace ES;

int main(int argc, char *argv[]) {
    
    sivelab::GraphicsArgs args;
    args.process(argc, argv);
    FrameBuffer fb;
    fb.set_height_width(1000, 1000);

    ES::PointN<float,3> cam_pos{0,3.0,4.0};
    ES::VectorN<float,3> cam_dir{0,-1.5,-3.0};
    float focal$ = .4f;
    float image_plane_width = .5f;
    std::size_t width = 1000;
    std::size_t height = 1000;
    PointLight light{{0,0,0},{1,1,1}};
    std::size_t rays_per_pixel;
    std::vector<PointLight> lights{light};
    int samples_per_pixel = 10; 
    ES::PerspectiveCamera p(cam_pos, cam_dir, focal$, image_plane_width, width, height);
    ShapeList scene({.3,.6,.7f});
    Renderer renderer(fb,p,lights);


// Ground Plane: triangle - large triangle at Z = 0, grey Lambertian
// Red Sphere: center=(-1.2, 1.0, -3.0), radius=1.0, Red Blinn Phong
// Mirror Sphere: center=(1.2, 1.10, -4.0), radius=1.10, Mirror;



    scene.add({Sphere({ -1.2,  1.0f, -3.f}, 1.0),BlinnPhongShader({1,0,0},100,1)});
    scene.add({Sphere({ 1.2,  1.10, -4.f}, 1.10),MirrorShader()});
    scene.add({Sphere({0.0f, -1001.0f, -3.0f}, 1000.0f), LambertianShader({0.5f, 0.5f, 0.5f}, 1.0f)});
    

    renderer(scene,50,0,INFINITY, 10);
    
    ES::export_as_PNG(renderer.fb, "defaultCamRayColors.png");
    
}