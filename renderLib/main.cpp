#include <cmath>
#include <iostream>
#include <chrono>  // <-- added for timing

#include "ES_color_util.hpp"
#include "ColorN.hpp"
#include "FrameBuffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Ray.hpp"
#include "Shader.hpp"
#include "Shape.hpp"
#include "SceneObject.hpp"
#include "Scene.hpp"
#include "PointLight.hpp"
#include "Renderer.hpp"
#include "../src/handleGraphicsArgs.h"
#include "readTriangleVerts.hpp"

using namespace ES;

int main(int argc, char *argv[]) {
    
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    // parsed arguments;
    std::string input_file = args.inputFileName;
    std::string output_file = args.outputFileName;
    std::size_t width = args.width;
    std::size_t height = args.height;
    float aspectRatio = args.aspectRatio;
    std::size_t  rays_per_pixel = args.rpp;
    std::size_t ray_depth;
    
    //non implemented;
    bool has_shadow;
    float depth_of_view_focus;
    auto split = 0;
    auto window_height = 0;
    auto window_width = 0;
    
    std::vector<float> vertices;
    readFloatsFromFile("trilist.dat",vertices);
    Mesh bunny(vertices);
    
    
    float image_plane_width = 1.0f;
    float focal = 1.0f;
    float fov = 2.0f * atan(image_plane_width / (2.0f * focal)) * 180.0f / math::pi<float>;
    

    
    FrameBuffer fb;
    fb.set_height_width(height, width);

    ES::PointN<float,3> cam_pos{0,0,2};
    ES::VectorN<float,3> cam_dir{0,0,-1};
    PointLight light{{1,5,1},{1,1,1}};
    PointLight light2{{-1,5,1},{1,1,1}};
    PointLight light3{{-1,5,-5},{1,1,1}};
    
    
    std::vector<PointLight> lights{light, light2,light3};

    ES::PerspectiveCamera p(cam_pos, cam_dir, focal, image_plane_width, width, height);
    Scene scene({.3,.6,.7f});
    scene.add_light(light).add_light(light2).add_light(light3);
    scene.add_camera(p);

    Renderer renderer(fb,p,scene.lights);

    scene.add_shape({bunny,LambertianShader{{1,0,0},1}});

    // --- Render timer start ---
    auto start_time = std::chrono::high_resolution_clock::now();

    renderer(scene,rays_per_pixel,0,INFINITY, 10);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Render completed in " << elapsed_seconds.count() << " seconds.\n";
    // --- Render timer end ---

    ES::export_as_PNG(renderer.fb, output_file);
}