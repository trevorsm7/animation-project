#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <iostream>

class Vector3
{
    friend class Matrix4;
    friend class Vector4;

    friend std::ostream& operator<<(std::ostream&, const Vector3&);
    friend std::istream& operator>>(std::istream&, Vector3&);

protected:
    float m_x, m_y, m_z;

public:
    Vector3() {} // don't waste time when allocating lists of empty vertices
    explicit Vector3(float s) : m_x(s), m_y(s), m_z(s) {};
    Vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

    float getX() const {return m_x;}
    float getY() const {return m_y;}
    float getZ() const {return m_z;}

    void setX(float x) {m_x = x;}
    void setY(float y) {m_y = y;}
    void setZ(float z) {m_z = z;}

    float *getPointer() {return &m_x;} // should make m_x, m_y, m_z into an array instead

    // Const functions (result returned in a copy)
    Vector3 operator+(const Vector3&) const;
    Vector3 operator-(const Vector3&) const;
    Vector3 operator*(float) const; // scale
    Vector3 operator/(float) const;
    bool operator==(const Vector3& v) const {return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z;}
    bool operator<(const Vector3& v) const {return (m_x + m_y + m_z) < (v.m_x + v.m_y + v.m_z);}
    float dot(const Vector3&) const;
    Vector3 cross(const Vector3&) const;
    float length() const;

    // In-place functions (result stored in this)
    Vector3& operator+=(const Vector3&);
    Vector3& operator-=(const Vector3&);
    Vector3& operator*=(float); // scale
    Vector3& operator/=(float);
    Vector3& negate(); // shortcut for *= -1.0
    Vector3& normalize();
    Vector3& min(const Vector3&);
    Vector3& max(const Vector3&);
};

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
    return out << v.m_x << " " << v.m_y << " " << v.m_z;
}

inline std::istream& operator>>(std::istream& in, Vector3& v)
{
    return in >> v.m_x >> v.m_y >> v.m_z;
}

#endif