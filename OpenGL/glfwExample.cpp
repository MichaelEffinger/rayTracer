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

int main(void)
{
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

    GLuint m_triangleVBO[2];

    ES::Vector3 V0(-3.0f, -3.0f, 0.f);
    ES::Vector3 V1(3.0f, -3.0f, 0.f);
    ES::Vector3 V2(0.0f, 3.0f, 0.f);

    ES::Triangle myTri{V0, V1, V2};

    glGenBuffers(1, m_triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

    std::vector<float> host_VertexBuffer = ES::to_buffer(myTri);

    glBufferData(GL_ARRAY_BUFFER,
                 host_VertexBuffer.size() * sizeof(float),
                 host_VertexBuffer.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    host_VertexBuffer.clear();

    GLuint m_VAO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    sivelab::GLSLObject shader;
    shader.addShader("vertexShader_PrepForPerFragment.glsl",
                     sivelab::GLSLObject::VERTEX_SHADER);
    shader.addShader("blinnPhong.glsl",            
                     sivelab::GLSLObject::FRAGMENT_SHADER);
    shader.createProgram();

    GLuint projMatrixID   = shader.createUniform("projMatrix");
    GLuint viewMatrixID   = shader.createUniform("viewMatrix");
    GLuint modelMatrixID  = shader.createUniform("modelMatrix");
    GLuint normalMatrixID = shader.createUniform("normalMatrix");
    GLuint lightPosID     = shader.createUniform("lightPosWorld");
    GLuint diffuseID      = shader.createUniform("diffuseComponent");
    GLuint specularID     = shader.createUniform("specularComponent");
    GLuint shininessID    = shader.createUniform("shininess");
    GLuint viewPosID      = shader.createUniform("viewPos");
    GLuint lightColorID   = shader.createUniform("lightColor");


    ES::Camera cam;
    cam.setPerspective(60.0f, aspectRatio, 0.1f, 100.0f);

    ES::VectorN<float,3> camPos(0, 0, 3);

    ES::VectorN<float,3> lightPos(0.f, 0.f, 10.f);

    shader.activate();
    glUniform3f(lightColorID, 1.0f, 1.0f, 1.0f);
    glUniform3f(specularID,   1.0f, 1.0f, 1.0f);
    glUniform1f(shininessID,  32.0f);
    glUniform3f(lightPosID,   lightPos[0], lightPos[1], lightPos[2]); 
    shader.deactivate();

    double startFrameTime = glfwGetTime();

    ES::AffineTransform3<float> model1Transform = ES::AffineTransform3<float>::from_translation({-.0f, .0f, 0.0f});
    ES::AffineTransform3<float> model2Transform = ES::AffineTransform3<float>::from_translation({0.f, .0f, 0.0f});

    ES::AffineTransform3<float> cameraTransform;

    cam.setLookAt(
        ES::PointN<float,3>{camPos[0], camPos[1], camPos[2]},
        ES::PointN<float,3>{camPos[0], camPos[1], camPos[2] - 1.0f},
        ES::VectorN<float,3>{0, 1, 0}
    );

    
    while (!glfwWindowShouldClose(window))
    {
        double endFrameTime = glfwGetTime();
        double timeDiff = endFrameTime - startFrameTime;
        startFrameTime = endFrameTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.activate();

        ES::Matrix<float,4> proj = cam.projectionMatrix();
        ES::Matrix<float,4> view = cam.viewMatrix();

        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, proj.ptr());
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, view.ptr());

        auto currentPos = cam.getPosition(); 
        glUniform3f(viewPosID, currentPos[0], currentPos[1], currentPos[2]);

        glBindVertexArray(m_VAO);

        ES::Matrix<float,4> model1 = model1Transform.to_matrix4();
        ES::Matrix<float,4> normalMat1 = model1; 

        glUniformMatrix4fv(modelMatrixID,  1, GL_FALSE, model1.ptr());
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, normalMat1.ptr());
        glUniform3f(diffuseID, 0.0f, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        ES::Matrix<float,4> model2 = model1Transform.to_matrix4();
        ES::Matrix<float,4> normalMat2 = model2; 

        glUniformMatrix4fv(modelMatrixID,  1, GL_FALSE, model2.ptr());
        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, normalMat2.ptr());
        glUniform3f(diffuseID, 1.0f, 0.0f, 0.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        shader.deactivate();

        glfwSwapBuffers(window);
        glfwPollEvents();

        float moveRate = 0.05f;

        ES::VectorN<float,3> forward = cam.getForward(); 
        ES::VectorN<float,3> right = cam.getRight();
        ES::VectorN<float,3> up = cam.getUp();

        

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
            cam.rotate(up, ES::Angle<ES::in_degrees,float>{-1});
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            cam.rotate(up, ES::Angle<ES::in_degrees,float>{1});
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            cam.rotate(right,ES::Angle<ES::in_degrees,float>{1});
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            cam.rotate(right,ES::Angle<ES::in_degrees,float>{-1});
        }  
        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
            cam.rotate(forward,ES::Angle<ES::in_degrees,float>{-1});
        }  
        if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS){
            cam.rotate(forward,ES::Angle<ES::in_degrees,float>{1});
        }  
    
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            std::cout << "fps: " << 1.0 / timeDiff << std::endl;
    }

    glfwTerminate();
    return 0;
}