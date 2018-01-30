#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include "Matrix4.h"

class Plane
{
protected:
    Vector3 m_n;
    float m_d;

public:
    Plane() {}
    Plane(const Vector3& normal, const Vector3& point): m_n(normal), m_d(point.dot(normal)) {}
    float distance(const Vector3& point) const {return point.dot(m_n) - m_d;}
};

class Frustum
{
protected:
    Matrix4 m_frustum;
    Plane m_planes[6];
    float m_halfFovX, m_halfFovY;

public:
    Frustum() {}
    void setProjection(float fovY, int width, int height, float near, float far);
    float* getPointer() {return m_frustum.getPointer();}
    Matrix4& getMatrix() {return m_frustum;}
    bool cull(const Vector3& center, float radius) const;
    
    float getHalfFovX() {return m_halfFovX;}
    float getHalfFovY() {return m_halfFovY;}
};

//extern Frustum g_frustum;

#endif
