#pragma once
#include "../Math/Matrix.h"

class Camera
{
    public:
        Camera(double viewAngle, double nearPlaneDistance, double farPlaneDistance);
        Matrix<4,4>& GetViewMat();
        Matrix<4,4>& GetProjectionMatrix();
    protected:
        Matrix<4,4> m_viewMat;
        Matrix<4,4> m_projMat;
};

class FPSCamera : public Camera
{
    public:
    FPSCamera(const Vector<3>& pos, double pitch, double yaw, 
                double viewAngle = 3.14159265359 / 2.0, double near = 0.1, double far = 1000);
};

class LookAtCamera : public Camera
{
    public:
    LookAtCamera(const Vector<3>& pos, Vector<3> target, Vector<3> up,
                double viewAngle = 3.14159265359 / 2.0, double near = 0.1, double far = 1000);
};