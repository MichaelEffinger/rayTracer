#include <cmath>
#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <memory>
#include "ES_color_util.hpp"
#include "ColorN.hpp"
#include "FrameBuffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Ray.hpp"
#include "Shader.hpp"
#include "Shape.hpp"
#include "SceneObject.hpp"
#include "virtual_shapes.hpp"

using namespace ES;

// Helper function to create a grid of variant-based spheres
std::vector<SceneObject> create_variant_scene(int total_spheres, float start_z) {
    std::vector<SceneObject> scene;
    
    // Calculate grid dimensions (roughly square)
    int spheres_per_row = static_cast<int>(std::sqrt(total_spheres));
    int num_rows = (total_spheres + spheres_per_row - 1) / spheres_per_row;
    
    float x_spacing = 3.5f;
    float z_spacing = 3.5f;
    float radius = 1.2f;
    float y_pos = 2.0f;
    
    float start_x = -(num_rows - 1) * x_spacing / 2.0f;
    
    std::vector<ES::Shader> shader_types = {
        ES::ConstantShader({1, 0, 0}),
        ES::RandomShader(),
        ES::NormalShader(),
        ES::LambertianShader({0, 1, 0}, 1),
        ES::BlinnPhongShader({0.1f, 0.1f, 0.1f}, {0.6f, 0.6f, 0.6f}, {1, 1, 1}, 32, 1),
        ES::RandomLambertianShader(1)
    };
    
    int sphere_count = 0;
    for (int row = 0; row < num_rows && sphere_count < total_spheres; ++row) {
        float x = start_x + row * x_spacing;
        for (int col = 0; col < spheres_per_row && sphere_count < total_spheres; ++col) {
            float z = start_z - col * z_spacing;
            const ES::Shader& shader_to_use = shader_types[sphere_count % shader_types.size()];
            scene.push_back({ES::Sphere({x, y_pos, z}, radius), shader_to_use});
            sphere_count++;
        }
    }
    
    return scene;
}

// Helper function to create a grid of virtual-based spheres
std::vector<slow::SceneObject> create_virtual_scene(int total_spheres, float start_z) {
    std::vector<slow::SceneObject> scene;
    
    // Calculate grid dimensions (roughly square)
    int spheres_per_row = static_cast<int>(std::sqrt(total_spheres));
    int num_rows = (total_spheres + spheres_per_row - 1) / spheres_per_row;
    
    float x_spacing = 3.5f;
    float z_spacing = 3.5f;
    float radius = 1.2f;
    float y_pos = 2.0f;
    
    float start_x = -(num_rows - 1) * x_spacing / 2.0f;
    
    int sphere_count = 0;
    for (int row = 0; row < num_rows && sphere_count < total_spheres; ++row) {
        float x = start_x + row * x_spacing;
        for (int col = 0; col < spheres_per_row && sphere_count < total_spheres; ++col) {
            float z = start_z - col * z_spacing;
            
            // Cycle through shader types
            std::unique_ptr<slow::IShader> shader;
            switch (sphere_count % 6) {
                case 0:
                    shader = std::make_unique<slow::ConstantShader>(RGB{1, 0, 0});
                    break;
                case 1:
                    shader = std::make_unique<slow::RandomShader>();
                    break;
                case 2:
                    shader = std::make_unique<slow::NormalShader>();
                    break;
                case 3:
                    shader = std::make_unique<slow::LambertianShader>(RGB{0, 1, 0}, 1);
                    break;
                case 4:
                    shader = std::make_unique<slow::BlinnPhongShader>(
                        RGB{0.1f, 0.1f, 0.1f}, RGB{0.6f, 0.6f, 0.6f}, RGB{1, 1, 1}, 32, 1);
                    break;
                case 5:
                    shader = std::make_unique<slow::RandomLambertianShader>(1);
                    break;
            }
            
            scene.push_back(slow::SceneObject(
                std::make_unique<slow::Sphere>(
                    PointN<float, 3>{x, y_pos, z}, radius, std::move(shader))));
            sphere_count++;
        }
    }
    
    return scene;
}

// Benchmark variant-based rendering
double benchmark_variant(int width, int height, int num_spheres) {
    FrameBuffer fb;
    fb.set_height_width(height, width);
    
    ES::PointN<float, 3> cam_pos{0, 20, -10};
    ES::VectorN<float, 3> cam_dir = ES::VectorN<float, 3>{0, 0, -20};
    cam_dir.normalize_in_place();
    
    float focal = 2.f;
    float image_plane_width = 8.f;
    
    ES::VectorN<float, 3> light_dir = {-1, -1, -1};
    light_dir.normalize_in_place();
    
    ES::PerspectiveCamera p(cam_pos, cam_dir, focal, image_plane_width, width, height);
    fb.gradiant_up({1, 1, 1}, {0, 0, 0});
    
    auto scene = create_variant_scene(num_spheres, -20.0f);
    
    auto start = std::chrono::high_resolution_clock::now();
    float TMAX = INFINITY;
    float TMIN = 0;
    // Render
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
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    ES::export_as_PNG(fb, "variant_benchmark_value.png");
    
    return elapsed.count();
}

// Benchmark virtual-based rendering
double benchmark_virtual(int width, int height, int num_spheres) {
    FrameBuffer fb;
    fb.set_height_width(height, width);
    
    ES::PointN<float, 3> cam_pos{0, 20, -10};
    ES::VectorN<float, 3> cam_dir = ES::VectorN<float, 3>{0, 0, -20};
    cam_dir.normalize_in_place();
    
    float focal = 2.f;
    float image_plane_width = 8.f;
    
    ES::VectorN<float, 3> light_dir = {-1, -1, -1};
    light_dir.normalize_in_place();
    
    ES::PerspectiveCamera p(cam_pos, cam_dir, focal, image_plane_width, width, height);
    fb.gradiant_up({1, 1, 1}, {0, 0, 0});
    
    auto scene = create_virtual_scene(num_spheres, -20.0f);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Render
    for (int x = 0; x < fb.height(); ++x) {
        for (int y = 0; y < fb.width(); ++y) {
            Ray<float> r = p.generateRay(y, x);
            float closest = INFINITY;
            bool hit_anything = false;
            std::size_t hit_index = 0;
            
            for (int i = 0; i < scene.size(); i++) {
                float t_hit;
                if (scene[i].obj->intersect(r, t_hit) && t_hit < closest) {
                    closest = t_hit;
                    hit_index = i;
                    hit_anything = true;
                }
            }
            
            if (hit_anything) {
                PointN<float, 3> hit_position = r.origin + closest * r.direction;
                VectorN<float, 3> normal = scene[hit_index].obj->normal(hit_position);
                fb(y, x) = scene[hit_index].obj->shader->shade(hit_position, normal, r, light_dir);
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    ES::export_as_PNG(fb, "virtual_benchmark_value.png");
    
    return elapsed.count();
}

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);
    
    const int width = 800;
    const int height = 800;
    const int num_spheres = 10000;
    
    std::cout << "========================================" << std::endl;
    std::cout << "Ray Tracer Performance Benchmark" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Resolution: " << width << "x" << height << std::endl;
    std::cout << "Number of spheres: " << num_spheres << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Run variant benchmark
    std::cout << "Running VARIANT-based benchmark..." << std::endl;
    double variant_time = benchmark_variant(width, height, num_spheres);
    std::cout << "✓ Variant rendering complete!" << std::endl;
    std::cout << "  Time: " << variant_time << " seconds" << std::endl;
    std::cout << "  Output: variant_benchmark.png\n" << std::endl;
    
    // Run virtual benchmark
    std::cout << "Running VIRTUAL-based benchmark..." << std::endl;
    double virtual_time = benchmark_virtual(width, height, num_spheres);
    std::cout << "✓ Virtual rendering complete!" << std::endl;
    std::cout << "  Time: " << virtual_time << " seconds" << std::endl;
    std::cout << "  Output: virtual_benchmark.png\n" << std::endl;
    
    // Results
    std::cout << "========================================" << std::endl;
    std::cout << "RESULTS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Variant time:  " << variant_time << " seconds" << std::endl;
    std::cout << "Virtual time:  " << virtual_time << " seconds" << std::endl;
    std::cout << "Difference:    " << (virtual_time - variant_time) << " seconds" << std::endl;
    std::cout << "Speedup:       " << (virtual_time / variant_time) << "x" << std::endl;
    
    if (variant_time < virtual_time) {
        double percent_faster = ((virtual_time - variant_time) / virtual_time) * 100.0;
        std::cout << "Winner:        VARIANT (" << percent_faster << "% faster)" << std::endl;
    } else {
        double percent_faster = ((variant_time - virtual_time) / variant_time) * 100.0;
        std::cout << "Winner:        VIRTUAL (" << percent_faster << "% faster)" << std::endl;
    }
    std::cout << "========================================" << std::endl;
    
    return 0;
}