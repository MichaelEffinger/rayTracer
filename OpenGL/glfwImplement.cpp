#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../renderLib/AffineTransform3.hpp"
#include "../renderLib/Triangle.hpp"
#include "GL_helpers.hpp"
#include "../renderLib/Camera.hpp"

#include "GLSL.h"
#include "Material.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "ResourceManager.hpp"
#include "../renderLib/Sphere.hpp"
#include "Texture.hpp"
#include "shape_converter.hpp"
#include "Material_helpers.hpp"
#include "World.hpp"

#include "../src/SceneParser_JSON.hpp"
#include "OpenGLLoader.hpp"

int main(void){

    std::string SHADER_PATH = "../../assets/shaders/";
    std::string MODEL_PATH  = "../../assets/models/";
    std::string TEXTURE_PATH = "../../assets/textures/";

    if (!glfwInit()) {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int winWidth = 1920;
    int winHeight = 1080;
    
    float aspectRatio = (float)winWidth / (float)winHeight;

    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.9f, 0.7f, 0.85f, 1.0f);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    ES::ResourceManager& res = ES::ResourceManager::getInstance();
    ES::Shader* myShader0   = res.getShader("BlinnPhong","vertexShader.glsl","BlinnPhongTex.glsl");
    ES::Shader* myShader   = res.getShader("BlinnPhong","vertexShader.glsl","BlinnPhong.glsl");
    ES::Shader* myLambert  = res.getShader("LambertianShader","vertexShader.glsl","Lambertian.glsl");
    ES::Shader* mySchlick = res.getShader("SchlickShader", "vertexShader.glsl","Schlick.glsl");

    ES::World world(fb_width, fb_height);

    // -------------------------- MODELS -------------------------
    auto chess = ES::load_model_from_obj("../../assets/models/Chess/Chess_Board.obj", myShader0);
    chess.worldTransform = ES::AffineTransform3<float>::from_translation({0,0,2}).to_matrix4();
    world.add_instance(chess);

    { // goombas
        auto goomba = ES::load_model_from_obj("../../assets/models/Goomba/goomba.obj", myShader0);

        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({.8,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({2.5,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({4.25,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({6.00,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({-.8,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({-2.5,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({-4.25,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
        goomba.worldTransform = ES::AffineTransform3<float>::from_trs({-6.00,1.5,-2},ES::Quaternion<float>::identity(),{.1,.1,.1}).to_matrix4();
        world.add_instance(goomba);
    }
    
    { // waddles
        auto waddle = ES::load_model_from_obj("../../assets/models/Waddle Dee/waddledee.obj", myShader0);

        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({.8,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({2.5,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({4.25,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({6.00,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({-.8,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({-2.5,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({-4.25,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        waddle.worldTransform = ES::AffineTransform3<float>::from_trs({-6.00,1.6,6.25},{0,0,1,0},{6.5,6.5,6.5}).to_matrix4();
        world.add_instance(waddle);
        
    
    }

    { // gordo
        auto gordo = ES::load_model_from_obj("../../assets/models/gordo/DolGordo.obj", myShader0);


        gordo.worldTransform = ES::AffineTransform3<float>::from_trs({2.5,1.5,8},{0,0,1,0},{.15,.15,.15}).to_matrix4();
        world.add_instance(gordo);
        gordo.worldTransform = ES::AffineTransform3<float>::from_trs({-2.5,1.5,8},ES::Quaternion<float>{0,0,1,0}*ES::Quaternion<float>{0.86602540,0,-0.5,0},{.15,.15,.15}).to_matrix4();
        world.add_instance(gordo);
    }


    { // DeeDeeDee and Bonkers
        auto dedede = ES::load_model_from_obj("../../assets/models/King Dedede/dededetroph.obj", myShader0);
        dedede.worldTransform = ES::AffineTransform3<float>::from_trs({-6,1.5,8.5},{0,0,1,0},{.10,.10,.10}).to_matrix4();
        world.add_instance(dedede);

        auto bonkers = ES::load_model_from_obj("../../assets/models/bonkers/DolBonkers.obj", myShader0);
        bonkers.worldTransform = ES::AffineTransform3<float>::from_trs({6,1.5,8},ES::Quaternion<float>{0,0,1,0}*ES::Quaternion<float>{0.86602540,0,-0.5,0},{.15,.15,.15}).to_matrix4();
        world.add_instance(bonkers);
    }

    { // kibbles
        auto kibbles = ES::load_model_from_obj("../../assets/models/Sir/DolSirkibble.obj", myShader0);


        kibbles.worldTransform = ES::AffineTransform3<float>::from_trs({4.25,1.5,8},ES::Quaternion<float>{0,0,1,0}*ES::Quaternion<float>{0.86602540,0,-0.5,0},{.12,.12,.12}).to_matrix4();
        world.add_instance(kibbles);
        kibbles.worldTransform = ES::AffineTransform3<float>::from_trs({-4.25,1.5,8},ES::Quaternion<float>{0,0,1,0}*ES::Quaternion<float>{0.86602540,0,-0.5,0},{.12,.12,.12}).to_matrix4();
        world.add_instance(kibbles);

    }

    { // kirby and meta knight

        auto kirby = ES::load_model_from_obj("../../assets/models/Kirby/kirby.obj", myShader0);
        kirby.worldTransform = ES::AffineTransform3<float>::from_trs({.8,1.5,8},ES::Quaternion<float>{0,0,1,0},{.12,.12,.12}).to_matrix4();
        world.add_instance(kirby);


        auto meta = ES::load_model_from_obj("../../assets/models/Meta Knight/metaknight.obj", myShader0);
        meta.worldTransform = ES::AffineTransform3<float>::from_trs({-.8,1.5,8},ES::Quaternion<float>{0,0,1,0},{.08,.08,.08}).to_matrix4();
        world.add_instance(meta);


    }


    { // peach and mario 

        auto mario = ES::load_model_from_obj("../../assets/models/Mario/mariotroph.obj", myShader0);
        mario.worldTransform = ES::AffineTransform3<float>::from_trs({.8,1.5,-4},ES::Quaternion<float>::identity(),{.2,.2,.2}).to_matrix4();
        world.add_instance(mario);

        auto peach = ES::load_model_from_obj("../../assets/models/Peach/peach.obj", myShader0);
        peach.worldTransform = ES::AffineTransform3<float>::from_trs({-.8,1.5,-4},ES::Quaternion<float>::identity(),{.2,.2,.2}).to_matrix4();
        world.add_instance(peach);
 

    }

    { // Thwomps
        auto thwamp = ES::load_model_from_obj("../../assets/models/Thwomp/thwomp.obj", myShader0);

        thwamp.worldTransform = ES::AffineTransform3<float>::from_trs({6,1.5,-4},ES::Quaternion<float>::identity(),{.16,.16,.16}).to_matrix4();
        world.add_instance(thwamp);
        thwamp.worldTransform = ES::AffineTransform3<float>::from_trs({-6,1.5,-4},ES::Quaternion<float>::identity(),{.16,.16,.16}).to_matrix4();
        world.add_instance(thwamp);
    }

    { // Yohis

        auto yoshi = ES::load_model_from_obj("../../assets/models/Yoshi/yoshi.obj", myShader0);


        yoshi.worldTransform = ES::AffineTransform3<float>::from_trs({4.25,1.5,-4},ES::Quaternion<float>::identity(),{.15,.15,.15}).to_matrix4();
        world.add_instance(yoshi);
        yoshi.worldTransform = ES::AffineTransform3<float>::from_trs({-4.25,1.5,-4},ES::Quaternion<float>::identity(),{.15,.15,.15}).to_matrix4();
        world.add_instance(yoshi);
    }

    { // Toad
        auto toad = ES::load_model_from_obj("../../assets/models/Toad/toad.obj", myShader0);
        toad.worldTransform = ES::AffineTransform3<float>::from_trs({2.5,1.5,-4},ES::Quaternion<float>::identity(),{.15,.15,.15}).to_matrix4();
        world.add_instance(toad);
        toad.worldTransform = ES::AffineTransform3<float>::from_trs({-2.5,1.5,-4},ES::Quaternion<float>::identity(),{.15,.15,.15}).to_matrix4();
        world.add_instance(toad);
    }







    ES::Camera cam;
    cam.setPerspective(60.0f, aspectRatio, 0.1f, 100.0f);

    ES::VectorN<float,3> camPos(0,2,5);
    cam.setLookAt(
        {camPos[0],camPos[1],camPos[2]},
        {0,0,0},
        {0,1,0}
    );

    world.add_camera(cam);


    ES::PointN<float,3> lightPos(-5.f,5.f,5.f);
    ES::RGB lightColor(2.f,2.f,2.f);
    world.add_light({lightPos, lightColor});
    
    float globeRotationAngle = 0.0f;
    double startFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        double endFrameTime = glfwGetTime();
        double timeDiff = endFrameTime - startFrameTime;
        startFrameTime = endFrameTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        float moveRate = 0.05f;
        float DEGREE_COUNT = 1;

        ES::VectorN<float,3> forward = world.camera.getForward();
        ES::VectorN<float,3> right   = world.camera.getRight();
        ES::VectorN<float,3> up      = world.camera.getUp();

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            moveRate = 0.1f;
            DEGREE_COUNT = 2;
        }

        if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            moveRate = 0.1f;
            DEGREE_COUNT = 2;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            world.camera.translate(up*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            world.camera.translate(-up*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            world.camera.translate(-right*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            world.camera.translate(right*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            world.camera.translate(-forward*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
            world.camera.translate(forward*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            world.camera.rotate(up, ES::Angle<ES::in_degrees,float>{-DEGREE_COUNT});
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            world.camera.rotate(up, ES::Angle<ES::in_degrees,float>{DEGREE_COUNT});
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            world.camera.rotate(right,ES::Angle<ES::in_degrees,float>{DEGREE_COUNT});
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            world.camera.rotate(right,ES::Angle<ES::in_degrees,float>{-DEGREE_COUNT});
        }  
        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
            world.camera.rotate(forward,ES::Angle<ES::in_degrees,float>{-DEGREE_COUNT});
        }  
        if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS){
            world.camera.rotate(forward,ES::Angle<ES::in_degrees,float>{DEGREE_COUNT});
        }  
    }

    glfwTerminate();
    return 0;
}