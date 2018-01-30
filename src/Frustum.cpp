#include "Frustum.h"
#include "Vector3.h"
#include <cmath>

void Frustum::setProjection(float fovY, int width, int height, float near, float far)
{
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    m_frustum.projection(fovY, aspect, near, far);

    m_halfFovY = fovY / 2.0f * 3.141592653589793f / 180.0f;
    float topZ = sinf(m_halfFovY);
    float topY = cosf(m_halfFovY);
    m_halfFovX = aspect * m_halfFovY;
    float rightZ = sinf(m_halfFovX);
    float rightX = cosf(m_halfFovX);

    m_planes[0] = Plane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -near));
    m_planes[1] = Plane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, -far));
    m_planes[2] = Plane(Vector3(0.0f, topY, topZ), Vector3(0.0f, 0.0f, 0.0f));
    m_planes[3] = Plane(Vector3(0.0f, -topY, topZ), Vector3(0.0f, 0.0f, 0.0f));
    m_planes[4] = Plane(Vector3(rightX, 0.0f, rightZ), Vector3(0.0f, 0.0f, 0.0f));
    m_planes[5] = Plane(Vector3(-rightX, 0.0f, rightZ), Vector3(0.0f, 0.0f, 0.0f));
}

bool Frustum::cull(const Vector3& center, float radius) const
{
    for (int i = 1; i < 6; ++i) // skip near plane due to weird bug!
    {
        float distance = m_planes[i].distance(center);
        if (distance > radius)
        {
            //std::cout << "Culling: " << center << ", " << radius << "; dist: " << distance << ", plane: " << i << std::endl;
            return true;
        }
    }

    return false;
}
