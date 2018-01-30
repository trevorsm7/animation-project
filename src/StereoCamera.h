#ifndef __STEREOCAMERA_H__
#define __STEREOCAMERA_H__

#include "Matrix4.h"

class StereoCamera
{
protected:
    Matrix4 m_left, m_right;
    float m_screenWidth, m_screenHeight;
    float m_ocularDistance;

public:
    StereoCamera(float w, float h, float d): m_screenWidth(w), m_screenHeight(h), m_ocularDistance(d) {}
    
    void makeFrustum(float near, float screen, float far);
    
    float* getLeftPointer() {return m_left.getPointer();}
    float* getRightPointer() {return m_right.getPointer();}
    
    Matrix4& getLeftMatrix() {return m_left;}
    Matrix4& getRightMatrix() {return m_right;}
};

#endif
