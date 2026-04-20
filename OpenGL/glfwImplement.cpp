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
#include "ResourceManager.hpp"
#include "../renderLib/Sphere.hpp"
#include "shape_converter.hpp"
#include "Material_helpers.hpp"
int main(void){
    if (!glfwInit()) {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int winWidth = 1000;
    float aspectRatio = 1.0f;
    int winHeight = winWidth / aspectRatio;

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
    ES::Shader* myShader = res.getShader("BlinnPhong","vertexBlinnPhong.glsl","fragmentBlinnPhong.glsl");
    ES::Shader* myLambert = res.getShader("LambertianShader","vertexLambertian.glsl","fragmentLambertian.glsl");

    ES::Model myModel = ES::load_model_from_obj("Mario.obj", myLambert);
    ES::Model myGhost = ES::load_model_from_obj("Gold Ghost.obj", myShader);
    ES::Model myLuigi = ES::load_model_from_obj("Luigi.obj",myShader);
    ES::Model myDonkey = ES::load_model_from_obj("D.K..obj",myShader);
    ES::Model myWario = ES::load_model_from_obj("Wario.obj",myShader);
    ES::Model myPatty = ES::load_model_from_obj("Patty Wagon.obj",myShader);

    ES::Sphere mySphere{{0,0,0},2};

    auto myThing = ES::to_Mesh(mySphere,5);
    auto blinner = ES::make_blinn_phong({1,0,0},32,{1,1,1});
    ES::Object superSphere{myThing,ES::Matrix<float,4>::identity(),&blinner};

    myModel.worldTransform = ES::AffineTransform3<float>::from_translation({0,0,2}).to_matrix4();
    myGhost.worldTransform = ES::AffineTransform3<float>::from_translation({2,2,2}).to_matrix4();
    myLuigi.worldTransform = ES::AffineTransform3<float>::from_translation({-2,-2, 2}).to_matrix4();
    myDonkey.worldTransform = ES::AffineTransform3<float>::from_trs({-2,2, 2},ES::Quaternion<float>::identity(),{.2,.2,.2}).to_matrix4();
    myWario.worldTransform = ES::AffineTransform3<float>::from_translation({-4, 0, 2}).to_matrix4();
    myPatty.worldTransform = ES::AffineTransform3<float>::from_translation({0,-5,2}).to_matrix4();

    ES::Camera cam;
    cam.setPerspective(60.0f,aspectRatio, 0.1f, 100.0f);
    ES::VectorN<float,3> camPos(0,2,5);
    cam.setLookAt({camPos[0],camPos[1], camPos[2]},{0,0,0},{0,1,0});


    ES::PointN<float,3> lightPos(5.f,5.f,5.f);
    ES::VectorN<float,3> lightColor(1.f,1.f,1.f);


    double startFrameTime = glfwGetTime();

    
    while (!glfwWindowShouldClose(window)){
        
        double endFrameTime = glfwGetTime();
        double timeDiff = endFrameTime - startFrameTime;
        startFrameTime = endFrameTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myModel.draw(cam.projectionMatrix(),cam.viewMatrix(),lightPos,cam.getPosition(),lightColor);
        myGhost.draw(cam.projectionMatrix(),cam.viewMatrix(),lightPos,cam.getPosition(),lightColor);
        myLuigi.draw(cam.projectionMatrix(),cam.viewMatrix(),lightPos,cam.getPosition(),lightColor);
        myDonkey.draw(cam.projectionMatrix(),cam.viewMatrix(),lightPos,cam.getPosition(),lightColor);
        myWario.draw(cam.projectionMatrix(),cam.viewMatrix(),lightPos,cam.getPosition(),lightColor);
        myPatty.draw(cam.projectionMatrix(),cam.viewMatrix(),lightPos,cam.getPosition(),lightColor);
        superSphere.draw(cam.projectionMatrix(),cam.viewMatrix(),lightPos,cam.getPosition(),lightColor);
        glfwSwapBuffers(window);

        glfwPollEvents();
        float moveRate = 0.05f;
        int DEGREE_COUNT = 1;

        ES::VectorN<float,3> forward = cam.getForward(); 
        ES::VectorN<float,3> right = cam.getRight();
        ES::VectorN<float,3> up = cam.getUp();

        if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            moveRate = 0.1f;
            DEGREE_COUNT = 2;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            cam.translate(up*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            cam.translate(-up*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            cam.translate(-right*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            cam.translate(right*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            cam.translate(-forward*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
            cam.translate(forward*moveRate);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            cam.rotate(up, ES::Angle<ES::in_degrees,float>{-DEGREE_COUNT});
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            cam.rotate(up, ES::Angle<ES::in_degrees,float>{DEGREE_COUNT});
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            cam.rotate(right,ES::Angle<ES::in_degrees,float>{DEGREE_COUNT});
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            cam.rotate(right,ES::Angle<ES::in_degrees,float>{-DEGREE_COUNT});
        }  
        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
            cam.rotate(forward,ES::Angle<ES::in_degrees,float>{-DEGREE_COUNT});
        }  
        if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS){
            cam.rotate(forward,ES::Angle<ES::in_degrees,float>{DEGREE_COUNT});
        }  
    
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            std::cout << "fps: " << 1.0 / timeDiff << std::endl;
    }

    glfwTerminate();
    return 0;
}