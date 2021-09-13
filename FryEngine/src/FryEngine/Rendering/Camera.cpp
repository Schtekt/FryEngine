#include "Camera.h"

Matrix<4,4>& Camera::GetViewMat()
{
    return m_viewMat;
}

Matrix<4,4> Camera::GetProjectionMatrix(double viewAngle, double near, double far)
{
    float scale = 1 / tan(viewAngle  / 2);
    Matrix<4,4> res;
    res.SetElement(0, 0, scale);
    res.SetElement(1, 1, scale);
    res.SetElement(2, 2, -far / (far - near));
    res.SetElement(3, 2, -far * near / (far - near));
    res.SetElement(2, 3, -1);
    res.SetElement(3, 3, 0);
    return res;
}

// FPS camera
FPSCamera::FPSCamera(const Vector<3>& pos, double pitch, double yaw)
{
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);
 
    Vector<3> xAxis = { cosYaw, 0, -sinYaw };
    Vector<3> yAxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
    Vector<3> zAxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

    m_viewMat = 
    {
        xAxis.nums[0], yAxis.nums[0], zAxis.nums[0], 0,
        xAxis.nums[1], yAxis.nums[1], zAxis.nums[1], 0,
        xAxis.nums[2], yAxis.nums[2], zAxis.nums[2], 0,
        -xAxis.Dot(pos), -yAxis.Dot(pos), -zAxis.Dot(pos), 1
    };
}

//Look at camera
LookAtCamera::LookAtCamera(const Vector<3>& pos, Vector<3> target, Vector<3> up)
{
    Vector<3> zAxis = (pos - target);
    zAxis /= zAxis.Length(); // normalize

    Vector<3> xAxis = up.Cross(zAxis);
    xAxis /= xAxis.Length(); // normalize

    Vector<3> yAxis = zAxis.Cross(xAxis);
    // normalized crossed with normalized gives normalized vector

    m_viewMat = 
    {
        xAxis.nums[0], yAxis.nums[0], zAxis.nums[0], 0,
        xAxis.nums[1], yAxis.nums[1], zAxis.nums[1], 0,
        xAxis.nums[2], yAxis.nums[2], zAxis.nums[2], 0,
        -xAxis.Dot(pos), -yAxis.Dot(pos), -zAxis.Dot(pos), 1
    };
}