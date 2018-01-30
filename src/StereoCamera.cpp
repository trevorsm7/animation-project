#include "StereoCamera.h"
#include "Vector3.h"
#include <cmath>
#include <cstdio>

void StereoCamera::makeFrustum(float near, float screen, float far)
{
    float l, r, b, t;
    
    // bottom and top are same for both
    t = 0.5f * m_screenHeight * near / screen;
    b = -t;
    
    float eye = m_ocularDistance * .5f;
    float edge = m_screenWidth * .5f;
    float ratio = near / screen;
    
    l = -(edge - eye) * ratio;
    r = (edge + eye) * ratio;
    printf("Left: %f, %f, %f, %f, %f, %f\n", l, r, b, t, near, far);
    m_left.frustum(l, r, b, t, near, far);
    m_left *= Matrix4().translate(eye, 0.f, 0.f);
    
    l = -(edge + eye) * ratio;
    r = (edge - eye) * ratio;
    printf("Right: %f, %f, %f, %f, %f, %f\n", l, r, b, t, near, far);
    m_right.frustum(l, r, b, t, near, far);
    m_right *= Matrix4().translate(-eye, 0.f, 0.f);
}
