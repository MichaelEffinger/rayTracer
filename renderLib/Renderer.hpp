#include "ColorN.hpp"
#include "FrameBuffer.hpp"
#include "PerspectiveCamera.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "PointLight.hpp"


namespace ES{

    class Renderer{
        
        public:

        FrameBuffer& fb;
        const RayCamera& camera;
        const std::vector<PointLight>& lights;


        Renderer(FrameBuffer& buffer, RayCamera& camera_, std::vector<PointLight>& lights_) noexcept : fb{buffer}, camera{camera_}, lights{lights_} {}

        void operator()(const Scene& shapes, std::size_t samples_per_pixel, float TMIN, float TMAX, std::size_t depth) const noexcept {
            std::mt19937 rng;
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);

            for (int x = 0; x < fb.width(); ++x) {
                for (int y = 0; y < fb.height(); ++y) {
                    RGB accumulated{0, 0, 0};

                    for (std::size_t s = 0; s < samples_per_pixel; ++s) {
                        Ray<float> r = camera.generateRay(y + dist(rng), x + dist(rng));
                        float t_max = TMAX;
                        Hit hit;
                        std::size_t hit_index = 0;

                        if (shapes.intersect(r, TMIN, t_max, hit, hit_index)) {
                            accumulated += shader::shade(shapes.shapes[hit_index].shade, hit, r, depth, shapes);
                        } else {
                            accumulated += shapes.background;
                        }
                    }

                    fb(y, x) = accumulated * (1.0f / samples_per_pixel);
                }
            }
        }


    };
}