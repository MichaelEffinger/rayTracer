#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../renderLib/ES_math.hpp"
#include "../renderLib/Ray.hpp"
#include "../renderLib/VectorN.hpp"
#include "../renderLib/Sphere.hpp"
#include "../renderLib/Hit.hpp"
using namespace ES;

TEST_CASE("Sphere intersect basic hit", "[Sphere]"){
    Sphere sphere;
    sphere.origin = Vector3<float>(0.0f, 0.0f, 0.0f);
    sphere.radius = 1.0f;
    
    Ray<float> ray;
    ray.origin = Vector3<float>(0.0f, 0.0f, -5.0f);
    ray.direction = Vector3<float>(0.0f, 0.0f, 1.0f);
    
    float t_min = 0.0f;
    float t_max = 1000.0f;
    Hit hit;
    
    bool intersected = sphere.intersect(ray, t_min, t_max, hit);
    
    REQUIRE(intersected == true);
    REQUIRE(math::approx_equal(hit.t, 4.0f));
    REQUIRE(math::approx_equal(hit.normal[2], -1.0f));
}

TEST_CASE("Sphere intersect miss", "[Sphere]"){
    Sphere sphere;
    sphere.origin = Vector3<float>(0.0f, 0.0f, 0.0f);
    sphere.radius = 1.0f;
    
    Ray<float> ray;
    ray.origin = Vector3<float>(5.0f, 5.0f, 5.0f);
    ray.direction = Vector3<float>(1.0f, 0.0f, 0.0f);
    
    float t_min = 0.0f;
    float t_max = 1000.0f;
    Hit hit;
    
    bool intersected = sphere.intersect(ray, t_min, t_max, hit);
    
    REQUIRE(intersected == false);
}

TEST_CASE("Sphere intersect t_max boundary", "[Sphere]"){
    Sphere sphere;
    sphere.origin = Vector3<float>(0.0f, 0.0f, 0.0f);
    sphere.radius = 1.0f;
    
    Ray<float> ray;
    ray.origin = Vector3<float>(0.0f, 0.0f, -5.0f);
    ray.direction = Vector3<float>(0.0f, 0.0f, 1.0f);
    
    float t_min = 0.0f;
    float t_max = 3.0f;
    Hit hit;
    
    bool intersected = sphere.intersect(ray, t_min, t_max, hit);
    
    REQUIRE(intersected == false);
}

TEST_CASE("Sphere intersect from inside", "[Sphere]"){
    Sphere sphere;
    sphere.origin = Vector3<float>(0.0f, 0.0f, 0.0f);
    sphere.radius = 1.0f;
    
    Ray<float> ray;
    ray.origin = Vector3<float>(0.0f, 0.0f, 0.0f);
    ray.direction = Vector3<float>(0.0f, 0.0f, 1.0f);
    
    float t_min = 0.0f;
    float t_max = 1000.0f;
    Hit hit;
    
    bool intersected = sphere.intersect(ray, t_min, t_max, hit);
    
    REQUIRE(intersected == true);
    REQUIRE(math::approx_equal(hit.t, 1.0f));
}