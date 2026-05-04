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

    int winWidth = 1000;
    float aspectRatio = 1.0f;
    int winHeight = static_cast<int>(winWidth / aspectRatio);

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
    ES::Shader* myShader0   = res.getShader("BlinnPhong","Spaghetti.glsl","BlinnPhongTex.glsl");
    ES::Shader* myShader   = res.getShader("BlinnPhong","Spaghetti.glsl","BlinnPhong.glsl");
    ES::Shader* myLambert  = res.getShader("LambertianShader","Spaghetti.glsl","Lambertian.glsl");
    ES::Shader* mySchlick = res.getShader("SchlickShader", "Spaghetti.glsl","Schlick.glsl");

    ES::World world(fb_width, fb_height);
    
    auto myModel = ES::load_model_from_obj("../../assets/models/Mario/Mario.obj", myShader0);
    myModel.worldTransform = ES::AffineTransform3<float>::from_translation({0,0,2}).to_matrix4();
    world.add_instance(myModel);


    ES::Model myGhost = ES::load_model_from_obj("../../assets/models/ghost/Ghost.obj", myShader0);
    myGhost.worldTransform = ES::AffineTransform3<float>::from_translation({2,2,2}).to_matrix4();
    world.add_instance(myGhost);

    ES::Model myLuigi = ES::load_model_from_obj("../../assets/models/Luigi/Luigi.obj", myShader0);
    myLuigi.worldTransform = ES::AffineTransform3<float>::from_translation({-2,-2,2}).to_matrix4();
    world.add_instance(myLuigi);

    ES::Model myDonkey = ES::load_model_from_obj("../../assets/models/donkey/D.K..obj", myShader0);
    myDonkey.worldTransform = ES::AffineTransform3<float>::from_trs({-2,2,2}, ES::Quaternion<float>::identity(), {.2f,.2f,.2f}).to_matrix4();
    world.add_instance(myDonkey);

    ES::Model myWario = ES::load_model_from_obj("../../assets/models/Wario/Wario.obj", myShader0);
    myWario.worldTransform = ES::AffineTransform3<float>::from_translation({-5,-5,5}).to_matrix4();
    world.add_instance(myWario);

    ES::Model myPatty = ES::load_model_from_obj("../../assets/models/patty/Patty_Wagon.obj", myShader0);

    myPatty.worldTransform = ES::AffineTransform3<float>::from_translation({0,-5,2}).to_matrix4();
    world.add_instance(myPatty);


    ES::Sphere mySphere{{0,0,0},2};
    auto myThing = ES::to_Mesh(mySphere,30);

    ES::Texture theGlobe("../../assets/textures/earth_daymap_2k.png");
    ES::Texture theSpecularGlobe("../../assets/textures/earth_specular_map_2k.png");

    std::unordered_map<ES::TextureType, ES::Texture*> worldTextures;

    worldTextures[ES::TextureType::DIFFUSE] = &theGlobe;
    worldTextures[ES::TextureType::SPECULAR] = &theSpecularGlobe;
   
    ES::Material mymatter{myShader0,{0.7f, 0.7f, 0.7f, 0.1f, 0.1f, 0.1f, 15.0f, 1.0f},worldTextures};


    ES::Material slick;
    slick.shader = mySchlick; 
    slick.data ={1,.82,0,.5,.5,0,0,0};

    ES::Object superSphere{
        myThing,
        ES::Matrix<float, 4>::identity(),
        &mymatter
    };

    ES::Object sSphere{
        myThing,
        ES::AffineTransform3<float>::from_translation({2,2,2}).to_matrix4(),
        &slick
    };


    ES::Model theSchlick{{sSphere},ES::Matrix<float,4>::identity()};
    ES::Model theWholeGlobe{{superSphere},ES::Matrix<float,4>::identity()};

    // world.add_instance(theWholeGlobe);
    // world.add_instance(theSchlick);

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


        std::string datasource = R"({
        "shader": [
            { "_name": "white_chalk",  "_type": "Lambertian", "diffuse": [1, 1, 1] },
            { "_name": "red_chalk",    "_type": "Lambertian", "diffuse": [1, 0, 0] },
            { "_name": "yellow_chalk", "_type": "Lambertian", "diffuse": [1, 1, 0] },
            { "_name": "blue_chalk",   "_type": "Lambertian", "diffuse": [0, 0, 1] }
        ],
        "shape": [
            { "shader": { "_ref": "white_chalk"  }, "center": "-1.5 -1.5 -5", "radius": 1, "_name": "sphere1", "_type": "sphere" },
            { "shader": { "_ref": "red_chalk"    }, "center": "1.5 -1.5 -5",  "radius": 1, "_name": "sphere2", "_type": "sphere" },
            { "shader": { "_ref": "yellow_chalk" }, "center": "1.5 1.5 -5",   "radius": 1, "_name": "sphere3", "_type": "sphere" },
            { "shader": { "_ref": "blue_chalk"   }, "center": "-1.5 1.5 -5",  "radius": 1, "_name": "sphere4", "_type": "sphere" }
        ]
    })";
    
    std::shared_ptr<OpenGLSceneLoader> sload(new OpenGLSceneLoader(world));
    SceneParser_JSON sceneParser(sload);
    sceneParser.parseStringData(datasource);

    
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