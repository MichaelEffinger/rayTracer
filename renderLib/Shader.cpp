#include "ShaderVariant.hpp"
#include "Shader.hpp"
#include "ShapeList.hpp"

namespace ES {

    namespace shader {
        RGB shade(const Shader& shader_, const Hit& hit, const Ray<float>& ray,
                  const std::vector<PointLight>& lights, std::size_t depth,
                  const ShapeList& scene) noexcept {
            if (depth > 0) {
                return std::visit([&](auto&& sh) -> RGB { return sh(hit, ray, lights, depth, scene); }, shader_);
            }
            return {0, 0, 0};
        }
    }

    // --- ConstantShader ---
    ConstantShader::ConstantShader(RGB newColor) : color(newColor) {}

    constexpr RGB ConstantShader::operator()(const Hit& hit, const Ray<float>& ray,
        const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept {
        return color;
    }

    // --- RandomShader ---
    RandomShader::RandomShader() {}

    RGB RandomShader::operator()(const Hit& hit, const Ray<float>& ray,
        const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept {
        return { dist(rng), dist(rng), dist(rng) };
    }

    // --- LambertianShader ---
    LambertianShader::LambertianShader(RGB newColor, float intensity_)
        : color(newColor), intensity(intensity_) {}

    constexpr RGB LambertianShader::operator()(const Hit& hit, const Ray<float>& ray,
        const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept {
        RGB finalColor{0, 0, 0};
        for (const auto& light : lights) {
            VectorN<float,3> light_direction = (light.position - hit.position).normalize();
            float factor = std::max(0.0f, hit.normal.dot(light_direction));
            finalColor += (color * intensity) * light.intensity * factor;
        }
        return finalColor;
    }

    // --- RandomLambertianShader ---
    RGB RandomLambertianShader::operator()(const Hit& hit, const Ray<float>& ray,
        const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept {
        RGB finalColor{0, 0, 0};
        for (const auto& light : lights) {
            VectorN<float,3> light_direction = (light.position - hit.position).normalize();
            float factor = std::max(0.0f, hit.normal.dot(light_direction));
            finalColor += (RGB{dist(rng), dist(rng), dist(rng)} * intensity) * light.intensity * factor;
        }
        return finalColor;
    }

    // --- NormalShader ---
    NormalShader::NormalShader() {}

    constexpr RGB NormalShader::operator()(const Hit& hit, const Ray<float>& ray,
        const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept {
        return RGB{
            0.5f * (hit.normal.x() + 1.f),
            0.5f * (hit.normal.y() + 1.f),
            0.5f * (hit.normal.z() + 1.f)
        };
    }

    // --- BlinnPhongShader ---
    BlinnPhongShader::BlinnPhongShader(RGB color_, float shininess_, float intensity_, RGB specular_color_) noexcept
        : color(color_), shininess(shininess_), intensity(intensity_), specular_color(specular_color_) {}

    RGB BlinnPhongShader::operator()(const Hit& hit, const Ray<float>& ray,
        const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept {
        RGB finalColor{0, 0, 0};
        for (const auto& light : lights) {
            VectorN<float,3> L = (light.position - hit.position).normalize();
            VectorN<float,3> V = (ray.origin - hit.position).normalize();
            VectorN<float,3> H = (L + V).normalize();

            float diffuse_factor = std::fmax(0.0f, hit.normal.dot(L));
            RGB diffuse = (color * intensity) * light.intensity * diffuse_factor;

            float shine_factor = std::max(0.0f, hit.normal.dot(H));
            float specular_intensity = std::pow(shine_factor, shininess);
            RGB specular = specular_color * light.intensity * specular_intensity;

            finalColor += diffuse + specular;
        }
        return finalColor;
    }

    // --- MirrorShader ---
    MirrorShader::MirrorShader() noexcept{}

    RGB MirrorShader::operator()(const Hit& hit, const Ray<float>& ray,
        const std::vector<PointLight>& lights, std::size_t depth, const ShapeList& scene) const noexcept {
        if (depth == 0) return scene.background;

        VectorN<float,3> R = ray.direction - (2.f * ray.direction.dot(hit.normal) * hit.normal);
        Ray reflectedRay{ hit.position + math::default_epsilon<float>::value * R, R };

        Hit newHit;
        std::size_t hit_index = 0;
        float TMAX = INFINITY;
        if (scene.intersect(reflectedRay, 0.0f, TMAX, newHit, hit_index)) {
            return shader::shade(scene.data_[hit_index].shade, newHit, reflectedRay, lights, depth - 1, scene);
        }
        return scene.background;
    }

} // namespace ES