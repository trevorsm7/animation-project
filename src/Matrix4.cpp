#include <math.h>

#include "Matrix4.h"

Matrix4 Matrix4::operator*(const Matrix4& m) const
{
    Matrix4 mult;

    // Each element is a dot product of a row/col
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            mult.m_mat[i][j] = getRow(j).dot(m.getCol(i));

    return mult;
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
    float x = getRow(0).dot(v);
    float y = getRow(1).dot(v);
    float z = getRow(2).dot(v);
    float w = getRow(3).dot(v);

    return Vector4(x, y, z, w);
}

Vector3 Matrix4::multiply(const Vector3& v, float w) const
{
    Vector4 v4(v, w);

    float x = getRow(0).dot(v4);
    float y = getRow(1).dot(v4);
    float z = getRow(2).dot(v4);
    // don't worry about homogenizing

    return Vector3(x, y, z);
}

Vector4 Matrix4::getRow(int row) const
{
    return Vector4(m_mat[0][row], m_mat[1][row], m_mat[2][row], m_mat[3][row]);
}

Vector4 Matrix4::getCol(int col) const
{
    return Vector4(m_mat[col][0], m_mat[col][1], m_mat[col][2], m_mat[col][3]);
}

Matrix4& Matrix4::transpose()
{
    // Swap elements in the following pattern:
    // x 1 2 3
    // 1 x 4 5
    // 2 4 x 6
    // 3 5 6 x
    for (int i = 0; i < 4; ++i)
    {
        for (int j = i + 1; j < 4; ++j)
        {
            float swap = m_mat[i][j];
            m_mat[i][j] = m_mat[j][i];
            m_mat[j][i] = swap;
        }
    }

    return *this;
}

Matrix4& Matrix4::invertAffine()
{
    // Transpose 3x3 rotation part
    for (int i = 0; i < 3; ++i)
    {
        for (int j = i + 1; j < 3; ++j)
        {
            float swap = m_mat[i][j];
            m_mat[i][j] = m_mat[j][i];
            m_mat[j][i] = swap;
        }
    }
    
    // Rotate/invert translation part
    Vector3 col = Vector3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
    m_mat[3][0] = -Vector3(m_mat[0][0], m_mat[1][0], m_mat[2][0]).dot(col);
    m_mat[3][1] = -Vector3(m_mat[0][1], m_mat[1][1], m_mat[2][1]).dot(col);
    m_mat[3][2] = -Vector3(m_mat[0][2], m_mat[1][2], m_mat[2][2]).dot(col);
    
    // These should already be set (otherwise it isn't affine!)
    //m_mat[0][3] = 0;
    //m_mat[1][3] = 0;
    //m_mat[2][3] = 0;
    //m_mat[3][3] = 1;
    
    return *this;
}

Matrix4& Matrix4::identity()
{
    zeros();

    m_mat[0][0] = 1;
    m_mat[1][1] = 1;
    m_mat[2][2] = 1;
    m_mat[3][3] = 1;

    return *this;
}

// m_rotationAngle in degrees
Matrix4& Matrix4::rotateX(float angle)
{
    // convert from degrees to radians
    angle = angle * 3.141592653589793f / 180.0f;
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    zeros();

    m_mat[0][0] = 1;
    m_mat[1][1] = cosAngle;
    m_mat[1][2] = -sinAngle;
    m_mat[2][1] = sinAngle;
    m_mat[2][2] = cosAngle;
    m_mat[3][3] = 1;

    return *this;
}

// m_rotationAngle in degrees
Matrix4& Matrix4::rotateY(float angle)
{
    // convert from degrees to radians
    angle = angle * 3.141592653589793f / 180.0f;
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    zeros();

    m_mat[0][0] = cosAngle;
    m_mat[0][2] = sinAngle;
    m_mat[1][1] = 1;
    m_mat[2][0] = -sinAngle;
    m_mat[2][2] = cosAngle;
    m_mat[3][3] = 1;

    return *this;
}

// m_rotationAngle in degrees
Matrix4& Matrix4::rotateZ(float angle)
{
    // convert from degrees to radians
    angle = angle * 3.141592653589793f / 180.0f;
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    zeros();

    m_mat[0][0] = cosAngle;
    m_mat[0][1] = -sinAngle;
    m_mat[1][0] = sinAngle;
    m_mat[1][1] = cosAngle;
    m_mat[2][2] = 1;
    m_mat[3][3] = 1;

    return *this;
}

// m_rotationAngle in degrees
Matrix4& Matrix4::rotate(float angle, const Vector3& axis)
{
    // convert from degrees to radians
    angle = angle * 3.141592653589793f / 180.0f;
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);

    zeros();

    m_mat[0][0] = axis.m_x * axis.m_x * (1 - cosAngle) + cosAngle;
    m_mat[1][1] = axis.m_y * axis.m_y * (1 - cosAngle) + cosAngle;
    m_mat[2][2] = axis.m_z * axis.m_z * (1 - cosAngle) + cosAngle;
    m_mat[0][1] = axis.m_x * axis.m_y * (1 - cosAngle) + axis.m_z * sinAngle;
    m_mat[1][0] = axis.m_x * axis.m_y * (1 - cosAngle) - axis.m_z * sinAngle;
    m_mat[0][2] = axis.m_x * axis.m_z * (1 - cosAngle) - axis.m_y * sinAngle;
    m_mat[2][0] = axis.m_x * axis.m_z * (1 - cosAngle) + axis.m_y * sinAngle;
    m_mat[1][2] = axis.m_y * axis.m_z * (1 - cosAngle) + axis.m_x * sinAngle;
    m_mat[2][1] = axis.m_y * axis.m_z * (1 - cosAngle) - axis.m_x * sinAngle;
    m_mat[3][3] = 1;

    return *this;
}

Matrix4& Matrix4::scale(float sx, float sy, float sz)
{
    zeros();

    m_mat[0][0] = sx;
    m_mat[1][1] = sy;
    m_mat[2][2] = sz;
    m_mat[3][3] = 1;

    return *this;
}

Matrix4& Matrix4::translate(float tx, float ty, float tz)
{
    zeros();

    m_mat[0][0] = 1;
    m_mat[1][1] = 1;
    m_mat[2][2] = 1;
    m_mat[3][0] = tx;
    m_mat[3][1] = ty;
    m_mat[3][2] = tz;
    m_mat[3][3] = 1;

    return *this;
}

Matrix4& Matrix4::ballJoint(const Vector3& r, const Vector3& t)
{
    // convert from degrees to radians
    //angle = angle * 3.141592653589793f / 180.0f;
    // jk, take angle in radians arbitrarily for CSE 169 assignment
    float cx = cosf(r.getX());
    float sx = sinf(r.getX());
    float cy = cosf(r.getY());
    float sy = sinf(r.getY());
    float cz = cosf(r.getZ());
    float sz = sinf(r.getZ());
    
    m_mat[0][0] = cy * cz;
    m_mat[0][1] = cy * sz;
    m_mat[0][2] = -sy;
    m_mat[0][3] = 0;
    m_mat[1][0] = sx * sy * cz - cx * sz;
    m_mat[1][1] = sx * sy * sz + cx * cz;
    m_mat[1][2] = sx * cy;
    m_mat[1][3] = 0;
    m_mat[2][0] = cx * sy * cz + sx * sz;
    m_mat[2][1] = cx * sy * sz - sx * cz;
    m_mat[2][2] = cx * cy;
    m_mat[2][3] = 0;
    m_mat[3][0] = t.getX();
    m_mat[3][1] = t.getY();
    m_mat[3][2] = t.getZ();
    m_mat[3][3] = 1;
    
    return *this;
}

Matrix4& Matrix4::camera(const Vector3& center, const Vector3& lookAt, const Vector3& up)
{
    Vector3 zaxis = (center - lookAt).normalize();
    Vector3 xaxis = up.cross(zaxis).normalize();
    Vector3 yaxis = zaxis.cross(xaxis);

    // so many non-zero, don't bother calling zeros()
    m_mat[0][0] = xaxis.m_x;
    m_mat[0][1] = yaxis.m_x;
    m_mat[0][2] = zaxis.m_x;
    m_mat[0][3] = 0.0f;
    m_mat[1][0] = xaxis.m_y;
    m_mat[1][1] = yaxis.m_y;
    m_mat[1][2] = zaxis.m_y;
    m_mat[1][3] = 0.0f;
    m_mat[2][0] = xaxis.m_z;
    m_mat[2][1] = yaxis.m_z;
    m_mat[2][2] = zaxis.m_z;
    m_mat[2][3] = 0.0f;
    m_mat[3][0] = -xaxis.dot(center);
    m_mat[3][1] = -yaxis.dot(center);
    m_mat[3][2] = -zaxis.dot(center);
    m_mat[3][3] = 1.0f;

    return *this;
}

Matrix4& Matrix4::frustum(float l, float r, float b, float t, float n, float f)
{
    zeros();

    m_mat[0][0] = 2.0f * n / (r - l);
    m_mat[1][1] = 2.0f * n / (t - b);
    m_mat[2][0] = (r + l) / (r - l);
    m_mat[2][1] = (t + b) / (t - b);
    m_mat[2][2] = -(f + n) / (f - n);
    m_mat[2][3] = -1.0f;
    m_mat[3][2] = -2.0f * f * n / (f - n);

    return *this;
}

Matrix4& Matrix4::projection(float fovY, float aspect, float near, float far)
{
    const float top = tanf(fovY / 2.0f * 3.141592653589793f / 180.0f) * near;
    const float right = top * aspect;

    return frustum(-right, right, -top, top, near, far);
}

Matrix4& Matrix4::ortho(float l, float r, float b, float t, float n, float f)
{
    zeros();

    m_mat[0][0]  = 2.0f / (r - l);
    m_mat[1][1]  = 2.0f / (t - b);
    m_mat[2][2] = -2.0f / (f - n);
    m_mat[3][3] = 1.0f;

    m_mat[3][0] = -(r + l) / (r - l);
    m_mat[3][1] = -(t + b) / (t - b);
    m_mat[3][2] = -(f + n) / (f - n);

    return *this;
}

Matrix4& Matrix4::viewport(int width, int height)
{
    zeros();

    m_mat[0][0] = width / 2;
    m_mat[3][0] = width / 2;
    m_mat[1][1] = height / 2;
    m_mat[3][1] = height / 2;
    m_mat[2][2] = 1.0f; // pass Z through as well for depth
    m_mat[3][3] = 1.0f;

    return *this;
}

Matrix4& Matrix4::bezier()
{
    zeros();

    m_mat[0][0] = -1.0f;
    m_mat[0][1] = 3.0f;
    m_mat[0][2] = -3.0f;
    m_mat[0][3] = 1.0f;
    m_mat[1][0] = 3.0f;
    m_mat[1][1] = -6.0f;
    m_mat[1][2] = 3.0f;
    m_mat[2][0] = -3.0f;
    m_mat[2][1] = 3.0f;
    m_mat[3][0] = 1.0f;

    return *this;
}