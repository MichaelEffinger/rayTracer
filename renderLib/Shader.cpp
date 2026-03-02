#include "ES_math.hpp"
#include "ShaderVariant.hpp"
#include "Shader.hpp"
#include "Scene.hpp"


#include <iostream>

namespace ES {

    namespace shader {
        RGB shade(const Shader& shader_, const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& scene) noexcept {
            if (depth > 0) {
                return std::visit([&](auto&& sh) -> RGB { return sh(hit, ray, depth, scene); }, shader_);
            }
            return {0, 0, 0};
        }


        float calculate_shadow(const Hit& hit, const PointLight& light, const Scene& scene){
            std::size_t hit_index;
            Hit shadow_hit; // fresh hit for shadow ray

            // Direction to the light
            auto to_light = light.position - hit.position;
            float tmax = to_light.magnitude();
            auto direction = to_light.normalize();

            // Shadow ray offset slightly to avoid self-intersection
            Ray<float> shadow_ray{
                hit.position + hit.normal * 0.0005f,
                direction
            };

            // Check if anything blocks the light
            bool blocked = scene.intersect(shadow_ray, 0.0005f, tmax, shadow_hit, hit_index);
            return blocked ? 1.0f : 0.0f; // 1 = blocked, 0 = visible
        }

    }

    // --- ConstantShader ---
    ConstantShader::ConstantShader(RGB newColor) : color(newColor) {}

    constexpr RGB ConstantShader::operator()(const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& scene) const noexcept {
        return color;
    }

    // --- RandomShader ---
    RandomShader::RandomShader() {}

    RGB RandomShader::operator()(const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& scene) const noexcept {
        return { dist(rng), dist(rng), dist(rng) };
    }

    // --- LambertianShader ---
    LambertianShader::LambertianShader(RGB newColor, float intensity_)
        : color(newColor), intensity(intensity_) {}

    constexpr RGB LambertianShader::operator()(const Hit& hit,const Ray<float>& ray,std::size_t depth, const Scene& scene) const noexcept{
        RGB finalColor{0, 0, 0};

        // Precompute view direction (optional for Lambertian, mostly used for Blinn-Phong)
        // VectorN<float,3> V = (ray.origin - hit.position).normalize();

        for (const auto& light : scene.lights){
            // 1️⃣ Direction to light
            VectorN<float,3> L = (light.position - hit.position).normalize();

            // 2️⃣ Diffuse factor (Lambert’s law)
            float diffuse_factor = std::max(0.0f, hit.normal.dot(L));
            RGB diffuse = (color * intensity) * light.intensity * diffuse_factor;

            float shadow = shader::calculate_shadow(hit, light, scene); // 0 = visible, 1 = blocked

            finalColor += diffuse * (1.0f - shadow);
        }

        return finalColor;
    }

    // --- NormalShader ---
    NormalShader::NormalShader() {}

    constexpr RGB NormalShader::operator()(const Hit& hit, const Ray<float>& ray,std::size_t depth, const Scene& scene) const noexcept {
        return RGB{
            0.5f * (hit.normal.x() + 1.f),
            0.5f * (hit.normal.y() + 1.f),
            0.5f * (hit.normal.z() + 1.f)
        };
    }

    // --- BlinnPhongShader ---
    BlinnPhongShader::BlinnPhongShader(RGB color_, float shininess_, float intensity_, RGB specular_color_) noexcept
        : color(color_), shininess(shininess_), intensity(intensity_), specular_color(specular_color_) {}

    RGB BlinnPhongShader::operator()(const Hit& hit,const Ray<float>& ray,std::size_t depth,const Scene& scene) const noexcept{
        RGB finalColor{0, 0, 0};
        VectorN<float,3> V = (ray.origin - hit.position).normalize();

        for (const auto& light : scene.lights){
            float shadow = shader::calculate_shadow(hit, light, scene);
            if (shadow >= 1.0f){
                continue;
            }
            VectorN<float,3> L = (light.position - hit.position).normalize();
            VectorN<float,3> H = (L + V).normalize();

            float diffuse_factor = std::fmax(0.0f, hit.normal.dot(L));
            RGB diffuse = (color * intensity) * light.intensity * diffuse_factor;

            float shine_factor = std::fmax(0.0f, hit.normal.dot(H));
            float specular_intensity = std::pow(shine_factor, shininess);
            RGB specular = specular_color * light.intensity * specular_intensity;

            finalColor += (diffuse + specular) * (1.0f - shadow);
        }

        return finalColor;
    }

    // --- MirrorShader ---
    MirrorShader::MirrorShader() noexcept{}

    RGB MirrorShader::operator()(const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& scene) const noexcept {
        if (depth == 0) return scene.background;

        VectorN<float,3> R = ray.direction - (2.f * ray.direction.dot(hit.normal) * hit.normal);
        Ray reflectedRay{ hit.position + math::default_epsilon<float>::value * R, R };

        Hit newHit;
        std::size_t hit_index = 0;
        float TMAX = INFINITY;
        if (scene.intersect(reflectedRay, 0.0005f, TMAX, newHit, hit_index)) {
            return shader::shade(scene.shapes[hit_index].shade, newHit, reflectedRay,depth - 1, scene);
        }
        return scene.background;
    }


    DiffuseShader::DiffuseShader(RGB newColor) noexcept: color(newColor){}
    RGB DiffuseShader::operator()(const Hit& hit, const Ray<float>& ray, std::size_t depth, const Scene& scene) const noexcept{
        if (depth == 0)
                return RGB{0,0,0};

            VectorN<float,3> N = hit.normal;
            PointN<float,3>  P = hit.position;

            VectorN<float,3> random_dir;
            while (true) {
                random_dir = { dist(rng), dist(rng), dist(rng) };

                if (random_dir.length_squared() < 1.0f){
                    break;
                }
            }
            random_dir.normalize_in_place(); 
            VectorN<float,3> scatter_dir = N + random_dir;

            if (scatter_dir.length_squared() < math::default_epsilon<float>::value)
                scatter_dir = N;

            scatter_dir.normalize_in_place();

            Ray<float> scattered(
                P + 0.001f * N,
                scatter_dir
            );

            Hit newHit;
            std::size_t hit_index = 0;
            float TMAX = INFINITY;

            if (scene.intersect(scattered, 0.001f, TMAX, newHit, hit_index)) {
                RGB bounced = shader::shade(
                    scene.shapes[hit_index].shade,
                    newHit,
                    scattered,
                    depth - 1,
                    scene
                );

                return .5f * color * bounced;
            }
            return .5f * color * scene.background; 
    }

} // namespace ES