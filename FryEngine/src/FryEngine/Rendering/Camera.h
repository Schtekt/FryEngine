#pragma once
#include "../Math/Matrix.h"

class Camera
{
    public:
        Matrix<4,4>& GetViewMat();
        static Matrix<4,4> GetProjectionMatrix(double viewAngle, double near, double far);
    protected:
        Matrix<4,4> m_viewMat;
};

class FPSCamera : public Camera
{
    public:
    FPSCamera(const Vector<3>& pos, double pitch, double yaw);
};

class LookAtCamera : public Camera
{
    public:
    LookAtCamera(const Vector<3>& pos, Vector<3> target, Vector<3> up);
};