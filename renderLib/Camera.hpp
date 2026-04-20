#pragma once
#include <cmath>
#include "AffineTransform3.hpp"
#include "Matrix.hpp"
#include "PointN.hpp"
#include "VectorN.hpp"
#include "Quaternion.hpp"
namespace ES {

class Camera {
private:
    PointN<float, 3> position;
    VectorN<float, 3> U, V, W;

    float fov = 60.0f;
    float aspect = 1.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    static float degToRad(float d) {
        return d * 3.1415926535f / 180.0f;
    }

public:

    Camera() = default;

    Camera(float fovDeg, float aspectRatio, float nearP, float farP): fov(fovDeg), aspect(aspectRatio), nearPlane(nearP), farPlane(farP) {
        position = PointN<float,3>{0,0,0};
        U = VectorN<float,3>{1,0,0};
        V = VectorN<float,3>{0,1,0};
        W = VectorN<float,3>{0,0,1};
    }

    Matrix<float,4> viewMatrix() const {
        Matrix<float,4> m = Matrix<float,4>::identity();

        m(0,0) = U[0]; m(0,1) = U[1]; m(0,2) = U[2];
        m(1,0) = V[0]; m(1,1) = V[1]; m(1,2) = V[2];
        m(2,0) = W[0]; m(2,1) = W[1]; m(2,2) = W[2];

        m(0,3) = -position.to_vector().dot(U);
        m(1,3) = -position.to_vector().dot(V);
        m(2,3) = -position.to_vector().dot(W);

        return m;
    }

    Matrix<float,4> projectionMatrix() const {
        Matrix<float,4> m{};
        float rad = degToRad(fov);
        float tanHalf = std::tan(rad * 0.5f);

        m(0,0) = 1.0f / (aspect * tanHalf);
        m(1,1) = 1.0f / tanHalf;

        m(2,2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
        m(3,2) = -1.0f;
        m(2,3) = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

        return m;
    }

    Matrix<float,4> viewProjectionMatrix() const {
        return projectionMatrix() * viewMatrix();
    }


    Camera& setLookAt(const PointN<float,3>& eye, const PointN<float,3>& target, const VectorN<float,3>& up){
        position = eye;

        VectorN<float,3> w = (eye - target).normalize();
        VectorN<float,3> u = up.cross(w).normalize();
        VectorN<float,3> v = w.cross(u);

        U = u;
        V = v;
        W = w;

        return *this;
    }

    Camera& setPerspective(float fovDeg, float a, float n, float f) {
        fov = fovDeg;
        aspect = a;
        nearPlane = n;
        farPlane = f;
        return *this;
    }


    Camera& rotate(Quaternion<float> quat){
        AffineTransform3<float> rotation = AffineTransform3<float>::from_rotation(quat);
        U = rotation.transform_vector(U);
        V = rotation.transform_vector(V);
        W = rotation.transform_vector(W);
        return *this;
    }

    Camera& rotate(VectorN<float,3> axis, Angle<in_radians,float> angle){
        Quaternion<float> temp;
        VectorN<float,3> normalized = axis.normalize();
        float angle_half = angle.get() * 0.5f;
        float sine = std::sin(angle_half);
        temp.w() = std::cos(angle_half);
        temp.x() = normalized.x()*sine;
        temp.y() = normalized.y()*sine;
        temp.z() = normalized.z()*sine; 
        rotate(temp);
        return *this;
    }
    

    Camera& translate(VectorN<float,3> translation){
        AffineTransform3<float> trans = AffineTransform3<float>::from_translation(translation);
        position = trans.transform_point(position);
        return *this;
    }
    
    const PointN<float,3>& getPosition() const { return position; }
    const VectorN<float,3>& getRight() const { return U; }
    const VectorN<float,3>& getUp() const { return V; }
    const VectorN<float,3>& getForward() const { return W; }
};

}