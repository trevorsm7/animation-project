#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Matrix4.h"

class Camera
{
protected:
    Vector3 m_center;
    Vector3 m_lookAt;
    Vector3 m_up;

public:
    Camera(): m_center(0.0f, 0.0f, 20.0f), m_lookAt(0.0f), m_up(0.0f, 1.0f, 0.0f) {}
    Camera(const Vector3& center, const Vector3& lookAt, const Vector3& up):
        m_center(center), m_lookAt(lookAt), m_up(up) {}

    Vector3& getCenter() {return m_center;}
    Vector3& getLookAt() {return m_lookAt;}
    Vector3& getUp() {return m_up;}

    // recompute matrix every time it is requested, since the vectors can be changed at will
    Matrix4 getMatrix()
    {
        Matrix4 matrix;
        matrix.camera(m_center, m_lookAt, m_up);
        return matrix;
    }
};

//extern Camera g_camera;

#endif
