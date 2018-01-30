#include "Vector3.h"
#include <iostream>
#include <cmath>

Vector3 Vector3::operator+(const Vector3& v) const
{
    float x = m_x + v.m_x;
    float y = m_y + v.m_y;
    float z = m_z + v.m_z;

    return Vector3(x, y, z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
    float x = m_x - v.m_x;
    float y = m_y - v.m_y;
    float z = m_z - v.m_z;

    return Vector3(x, y, z);
}

Vector3 Vector3::operator*(float s) const
{
    float x = m_x * s;
    float y = m_y * s;
    float z = m_z * s;

    return Vector3(x, y, z);
}

Vector3 Vector3::operator/(float s) const
{
    float x = m_x / s;
    float y = m_y / s;
    float z = m_z / s;

    return Vector3(x, y, z);
}

float Vector3::dot(const Vector3& v) const
{
    float x = m_x * v.m_x;
    float y = m_y * v.m_y;
    float z = m_z * v.m_z;

    return x + y + z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
    float x = m_y * v.m_z - m_z * v.m_y;
    float y = m_z * v.m_x - m_x * v.m_z;
    float z = m_x * v.m_y - m_y * v.m_x;

    return Vector3(x, y, z);
}

float Vector3::length() const
{
    float x_2 = m_x * m_x;
    float y_2 = m_y * m_y;
    float z_2 = m_z * m_z;

    return sqrtf(x_2 + y_2 + z_2);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z;

    return *this;
}

Vector3& Vector3::operator*=(float s)
{
    m_x *= s;
    m_y *= s;
    m_z *= s;

    return *this;
}

Vector3& Vector3::operator/=(float s)
{
    m_x /= s;
    m_y /= s;
    m_z /= s;

    return *this;
}

Vector3& Vector3::negate()
{
    m_x = -m_x;
    m_y = -m_y;
    m_z = -m_z;

    return *this;
}

Vector3& Vector3::normalize()
{
    float len = length();

    // Do not divide by zero!
    if (len > 0.0f)
        *this /= len;

    return *this;
}

Vector3& Vector3::min(const Vector3& v)
{
    m_x = fminf(m_x, v.m_x);
    m_y = fminf(m_y, v.m_y);
    m_z = fminf(m_z, v.m_z);

    return *this;
}

Vector3& Vector3::max(const Vector3& v)
{
    m_x = fmaxf(m_x, v.m_x);
    m_y = fmaxf(m_y, v.m_y);
    m_z = fmaxf(m_z, v.m_z);

    return *this;
}