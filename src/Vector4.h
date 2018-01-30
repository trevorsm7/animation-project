#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <iostream>
#include "Vector3.h"

class Vector4
{
    friend class Matrix4;
    friend class Vector3;

    friend std::ostream& operator<<(std::ostream&, const Vector4&);
    friend std::istream& operator>>(std::istream&, Vector4&);

protected:
    float m_x, m_y, m_z, m_w;

public:
    Vector4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
    Vector4(const Vector3& v, float w) : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z), m_w(w) {}
    explicit operator Vector3() const
    {
        if (m_w == 0.0f || m_w == 1.0f)
            return Vector3(m_x, m_y, m_z);

        return Vector3(m_x / m_w, m_y / m_w, m_z / m_w);
    }

    float getX() const {return m_x;}
    float getY() const {return m_y;}
    float getZ() const {return m_z;}
    float getW() const {return m_w;}

    // Const functions (result returned in a copy)
    Vector4 operator+(const Vector4&) const;
    Vector4 operator-(const Vector4&) const;
    float dot(const Vector4&) const;

    // In-place functions (result stored in this)
    Vector4& operator+=(const Vector4&);
    Vector4& operator-=(const Vector4&);
    Vector4& dehomogenize();
};

inline std::ostream& operator<<(std::ostream& out, const Vector4& v)
{
    return out << v.m_x << " " << v.m_y << " " << v.m_z << " " << v.m_w;
}

inline std::istream& operator>>(std::istream& in, Vector4& v)
{
    return in >> v.m_x >> v.m_y >> v.m_z >> v.m_w;
}

#endif