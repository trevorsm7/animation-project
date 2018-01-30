#include "Vector4.h"
#include <iostream>
#include <cmath>

Vector4 Vector4::operator+(const Vector4& v) const
{
    float x = m_x + v.m_x;
    float y = m_y + v.m_y;
    float z = m_z + v.m_z;
    float w = m_w + v.m_w;

    return Vector4(x, y, z, w);
}

Vector4 Vector4::operator-(const Vector4& v) const
{
    float x = m_x - v.m_x;
    float y = m_y - v.m_y;
    float z = m_z - v.m_z;
    float w = m_w - v.m_w;

    return Vector4(x, y, z, w);
}

float Vector4::dot(const Vector4& v) const
{
    float x = m_x * v.m_x;
    float y = m_y * v.m_y;
    float z = m_z * v.m_z;
    float w = m_w * v.m_w; // using this for matrix mutliplication

    return x + y + z + w;
}

Vector4& Vector4::operator+=(const Vector4& v)
{
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
    m_w += v.m_w;

    return *this;
}

Vector4& Vector4::operator-=(const Vector4& v)
{
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z;
    m_w -= v.m_w;

    return *this;
}

Vector4& Vector4::dehomogenize()
{
    m_x /= m_w;
    m_y /= m_w;
    m_z /= m_w;
    m_w = 1.0;

    return *this;
}