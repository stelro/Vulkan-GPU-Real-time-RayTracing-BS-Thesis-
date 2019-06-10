/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

//Engine Internal
#include "Matrix.hh"
#include "../core.hh"

//C++ Includes
#include <iostream>

namespace ost {

    Matrix4 Matrix4::compute_inverse(const Matrix4 &rhs)
    {
        Matrix4 result;
        float d = determinant();
        d = 1.0f / d;

        result[0][0] = d * (rhs[2][1] * rhs[3][2] * rhs[1][3]  -  rhs[3][1] * rhs[2][2] * rhs[1][3]  +  rhs[3][1] * rhs[1][2] * rhs[2][3]  -  rhs[1][1] * rhs[3][2] * rhs[2][3]  -  rhs[2][1] * rhs[1][2] * rhs[3][3]  +  rhs[1][1] * rhs[2][2] * rhs[3][3]);
        result[1][0] = d * (rhs[3][0] * rhs[2][2] * rhs[1][3]  -  rhs[2][0] * rhs[3][2] * rhs[1][3]  -  rhs[3][0] * rhs[1][2] * rhs[2][3]  +  rhs[1][0] * rhs[3][2] * rhs[2][3]  +  rhs[2][0] * rhs[1][2] * rhs[3][3]  -  rhs[1][0] * rhs[2][2] * rhs[3][3]);
        result[2][0] = d * (rhs[2][0] * rhs[3][1] * rhs[1][3]  -  rhs[3][0] * rhs[2][1] * rhs[1][3]  +  rhs[3][0] * rhs[1][1] * rhs[2][3]  -  rhs[1][0] * rhs[3][1] * rhs[2][3]  -  rhs[2][0] * rhs[1][1] * rhs[3][3]  +  rhs[1][0] * rhs[2][1] * rhs[3][3]);
        result[3][0] = d * (rhs[3][0] * rhs[2][1] * rhs[1][2]  -  rhs[2][0] * rhs[3][1] * rhs[1][2]  -  rhs[3][0] * rhs[1][1] * rhs[2][2]  +  rhs[1][0] * rhs[3][1] * rhs[2][2]  +  rhs[2][0] * rhs[1][1] * rhs[3][2]  -  rhs[1][0] * rhs[2][1] * rhs[3][2]);
        result[0][1] = d * (rhs[3][1] * rhs[2][2] * rhs[0][3]  -  rhs[2][1] * rhs[3][2] * rhs[0][3]  -  rhs[3][1] * rhs[0][2] * rhs[2][3]  +  rhs[0][1] * rhs[3][2] * rhs[2][3]  +  rhs[2][1] * rhs[0][2] * rhs[3][3]  -  rhs[0][1] * rhs[2][2] * rhs[3][3]);
        result[1][1] = d * (rhs[2][0] * rhs[3][2] * rhs[0][3]  -  rhs[3][0] * rhs[2][2] * rhs[0][3]  +  rhs[3][0] * rhs[0][2] * rhs[2][3]  -  rhs[0][0] * rhs[3][2] * rhs[2][3]  -  rhs[2][0] * rhs[0][2] * rhs[3][3]  +  rhs[0][0] * rhs[2][2] * rhs[3][3]);
        result[2][1] = d * (rhs[3][0] * rhs[2][1] * rhs[0][3]  -  rhs[2][0] * rhs[3][1] * rhs[0][3]  -  rhs[3][0] * rhs[0][1] * rhs[2][3]  +  rhs[0][0] * rhs[3][1] * rhs[2][3]  +  rhs[2][0] * rhs[0][1] * rhs[3][3]  -  rhs[0][0] * rhs[2][1] * rhs[3][3]);
        result[3][1] = d * (rhs[2][0] * rhs[3][1] * rhs[0][2]  -  rhs[3][0] * rhs[2][1] * rhs[0][2]  +  rhs[3][0] * rhs[0][1] * rhs[2][2]  -  rhs[0][0] * rhs[3][1] * rhs[2][2]  -  rhs[2][0] * rhs[0][1] * rhs[3][2]  +  rhs[0][0] * rhs[2][1] * rhs[3][2]);
        result[0][2] = d * (rhs[1][1] * rhs[3][2] * rhs[0][3]  -  rhs[3][1] * rhs[1][2] * rhs[0][3]  +  rhs[3][1] * rhs[0][2] * rhs[1][3]  -  rhs[0][1] * rhs[3][2] * rhs[1][3]  -  rhs[1][1] * rhs[0][2] * rhs[3][3]  +  rhs[0][1] * rhs[1][2] * rhs[3][3]);
        result[1][2] = d * (rhs[3][0] * rhs[1][2] * rhs[0][3]  -  rhs[1][0] * rhs[3][2] * rhs[0][3]  -  rhs[3][0] * rhs[0][2] * rhs[1][3]  +  rhs[0][0] * rhs[3][2] * rhs[1][3]  +  rhs[1][0] * rhs[0][2] * rhs[3][3]  -  rhs[0][0] * rhs[1][2] * rhs[3][3]);
        result[2][2] = d * (rhs[1][0] * rhs[3][1] * rhs[0][3]  -  rhs[3][0] * rhs[1][1] * rhs[0][3]  +  rhs[3][0] * rhs[0][1] * rhs[1][3]  -  rhs[0][0] * rhs[3][1] * rhs[1][3]  -  rhs[1][0] * rhs[0][1] * rhs[3][3]  +  rhs[0][0] * rhs[1][1] * rhs[3][3]);
        result[3][2] = d * (rhs[3][0] * rhs[1][1] * rhs[0][2]  -  rhs[1][0] * rhs[3][1] * rhs[0][2]  -  rhs[3][0] * rhs[0][1] * rhs[1][2]  +  rhs[0][0] * rhs[3][1] * rhs[1][2]  +  rhs[1][0] * rhs[0][1] * rhs[3][2]  -  rhs[0][0] * rhs[1][1] * rhs[3][2]);
        result[0][3] = d * (rhs[2][1] * rhs[1][2] * rhs[0][3]  -  rhs[1][1] * rhs[2][2] * rhs[0][3]  -  rhs[2][1] * rhs[0][2] * rhs[1][3]  +  rhs[0][1] * rhs[2][2] * rhs[1][3]  +  rhs[1][1] * rhs[0][2] * rhs[2][3]  -  rhs[0][1] * rhs[1][2] * rhs[2][3]);
        result[1][3] = d * (rhs[1][0] * rhs[2][2] * rhs[0][3]  -  rhs[2][0] * rhs[1][2] * rhs[0][3]  +  rhs[2][0] * rhs[0][2] * rhs[1][3]  -  rhs[0][0] * rhs[2][2] * rhs[1][3]  -  rhs[1][0] * rhs[0][2] * rhs[2][3]  +  rhs[0][0] * rhs[1][2] * rhs[2][3]);
        result[2][3] = d * (rhs[2][0] * rhs[1][1] * rhs[0][3]  -  rhs[1][0] * rhs[2][1] * rhs[0][3]  -  rhs[2][0] * rhs[0][1] * rhs[1][3]  +  rhs[0][0] * rhs[2][1] * rhs[1][3]  +  rhs[1][0] * rhs[0][1] * rhs[2][3]  -  rhs[0][0] * rhs[1][1] * rhs[2][3]);
        result[3][3] = d * (rhs[1][0] * rhs[2][1] * rhs[0][2]  -  rhs[2][0] * rhs[1][1] * rhs[0][2]  +  rhs[2][0] * rhs[0][1] * rhs[1][2]  -  rhs[0][0] * rhs[2][1] * rhs[1][2]  -  rhs[1][0] * rhs[0][1] * rhs[2][2]  +  rhs[0][0] * rhs[1][1]*rhs[2][2]);

        return result;
    }

    Matrix4::Matrix4() {

        this->m[0] = col_major(1, 0, 0, 0);
        this->m[1] = col_major(0, 1, 0, 0);
        this->m[2] = col_major(0, 0, 1, 0);
        this->m[3] = col_major(0, 0, 0, 1);
    }

    Matrix4::Matrix4(const Matrix4& rhs) {

        this->m[0] = rhs[0];
        this->m[1] = rhs[1];
        this->m[2] = rhs[2];
        this->m[3] = rhs[3];

    }

    Matrix4::Matrix4(float s) {
        this->m[0] = col_major(s, 0, 0, 0);
        this->m[1] = col_major(0, s, 0, 0);
        this->m[2] = col_major(0, 0, s, 0);
        this->m[3] = col_major(0, 0, 0, s);
    }

    Matrix4::Matrix4(const col_major& v1, const col_major& v2, const col_major& v3, const col_major& v4) {

        this->m[0] = v1;
        this->m[1] = v2;
        this->m[2] = v3;
        this->m[3] = v4;
    }

    Matrix4::Matrix4(
        float x0, float y0, float z0, float w0,
        float x1, float y1, float z1, float w1,
        float x2, float y2, float z2, float w2,
        float x3, float y3, float z3, float w3
    ) {

        this->m[0] = col_major(x0, y0, z0, w0);
        this->m[1] = col_major(x1, y1, z1, w1);
        this->m[2] = col_major(x2, y2, z2, w2);
        this->m[3] = col_major(x3, y3, z3, w3);
    }

    Matrix4::operator glm::mat4() {

        glm::mat4 result;

        result[0][0] = this->m[0][0]; result[0][1] = this->m[0][1]; result[0][2] = this->m[0][2]; result[0][3] = this->m[0][3];
        result[1][0] = this->m[1][0]; result[1][1] = this->m[1][1]; result[1][2] = this->m[1][2]; result[1][3] = this->m[1][3];
        result[2][0] = this->m[2][0]; result[2][1] = this->m[2][1]; result[2][2] = this->m[2][2]; result[2][3] = this->m[2][3];
        result[3][0] = this->m[3][0]; result[3][1] = this->m[3][1]; result[3][2] = this->m[3][2]; result[3][3] = this->m[3][3];

        return result;

    }

    typename Matrix4::col_major& Matrix4::operator [](size_t index) {

        OST_ASSERT_LOG(index < 4,"");
        return this->m[index];
    }

    typename Matrix4::col_major const& Matrix4::operator [](size_t index) const {

        OST_ASSERT_LOG(index < 4,"");
        return this->m[index];
    }


    Matrix4& Matrix4::operator =(const Matrix4& rhs) {

        this->m[0] = rhs[0];
        this->m[1] = rhs[1];
        this->m[2] = rhs[2];
        this->m[3] = rhs[3];

        return *this;
    }

    Matrix4& Matrix4::operator +=(float scalar) {

        this->m[0] += scalar;
        this->m[1] += scalar;
        this->m[2] += scalar;
        this->m[3] += scalar;
        return *this;
    }

    Matrix4& Matrix4::operator +=(const Matrix4& rhs) {

        this->m[0] += rhs[0];
        this->m[1] += rhs[1];
        this->m[2] += rhs[2];
        this->m[3] += rhs[3];
        return *this;

    }

    Matrix4& Matrix4::operator -=(float scalar) {

        this->m[0] -= scalar;
        this->m[1] -= scalar;
        this->m[2] -= scalar;
        this->m[3] -= scalar;
        return *this;
    }

    Matrix4& Matrix4::operator -=(const Matrix4& rhs) {

        this->m[0] -= rhs[0];
        this->m[1] -= rhs[1];
        this->m[2] -= rhs[2];
        this->m[3] -= rhs[3];
        return *this;

    }

    Matrix4& Matrix4::operator *=(float scalar) {

        this->m[0] *= scalar;
        this->m[1] *= scalar;
        this->m[2] *= scalar;
        this->m[3] *= scalar;
        return *this;

    }

    Matrix4& Matrix4::operator *=(const Matrix4& rhs) {

        return (*this = *this * rhs);
    }

    Matrix4& Matrix4::operator /=(float scalar) {

        this->m[0] /= scalar;
        this->m[1] /= scalar;
        this->m[2] /= scalar;
        this->m[3] /= scalar;
        return *this;
    }

    Matrix4& Matrix4::operator /=(const Matrix4& rhs) {

        return (*this = *this * compute_inverse(rhs));
    }

    Matrix4& Matrix4::operator ++() {

        ++this->m[0];
        ++this->m[1];
        ++this->m[2];
        ++this->m[3];
        return *this;
    }

    Matrix4& Matrix4::operator --() {

        --this->m[0];
        --this->m[1];
        --this->m[2];
        --this->m[3];
        return *this;
    }

    const Matrix4 Matrix4::operator ++(int) {

        Matrix4 result(*this);
        ++*this;
        return result;
    }

    const Matrix4 Matrix4::operator --(int) {

        Matrix4 result(*this);
        --*this;
        return result;
    }

    Matrix4 Matrix4::operator +(const Matrix4& rhs) {

        return rhs;
    }

    Matrix4 Matrix4::operator -(const Matrix4& rhs) {

        return Matrix4(
            -rhs[0][0], -rhs[0][1], -rhs[0][2], -rhs[0][3],
            -rhs[1][0], -rhs[1][1], -rhs[1][2], -rhs[1][3],
            -rhs[2][0], -rhs[2][1], -rhs[2][2], -rhs[2][3],
            -rhs[3][0], -rhs[3][1], -rhs[3][2], -rhs[3][3]
        );
    }

    Vec3 Matrix4::getZVector() const {

        return Vec3( this->m[2][0], this->m[2][1], this->m[2][2] );
    }

    Vec3 Matrix4::getYVector() const {

        return Vec3( this->m[1][0], this->m[1][1], this->m[1][2] );
    }

    Vec3 Matrix4::getXVector() const {

        return Vec3( this->m[0][0], this->m[0][1], this->m[0][2] );
    }

    void Matrix4::setXVector(const Vec3& v) {

        this->m[0][0] = v.x;
        this->m[0][1] = v.y;
        this->m[0][2] = v.z;
    }

    void Matrix4::setYVector(const Vec3& v) {

        this->m[1][0] = v.x;
        this->m[1][1] = v.y;
        this->m[1][2] = v.z;
    }

    void Matrix4::setZVector(const Vec3& v) {

        this->m[2][0] = v.x;
        this->m[2][1] = v.y;
        this->m[2][2] = v.z;
    }

    float Matrix4::determinant() const {

        return
            m[3][0] * m[2][1] * m[1][2] * m[0][3]  -  m[2][0] * m[3][1] * m[1][2] * m[0][3]  -  m[3][0] * m[1][1] * m[2][2] * m[0][3]  +  m[1][0] * m[3][1] * m[2][2] * m[0][3] +
            m[2][0] * m[1][1] * m[3][2] * m[0][3]  -  m[1][0] * m[2][1] * m[3][2] * m[0][3]  -  m[3][0] * m[2][1] * m[0][2] * m[1][3]  +  m[2][0] * m[3][1] * m[0][2] * m[1][3] +
            m[3][0] * m[0][1] * m[2][2] * m[1][3]  -  m[0][0] * m[3][1] * m[2][2] * m[1][3]  -  m[2][0] * m[0][1] * m[3][2] * m[1][3]  +  m[0][0] * m[2][1] * m[3][2] * m[1][3] +
            m[3][0] * m[1][1] * m[0][2] * m[2][3]  -  m[1][0] * m[3][1] * m[0][2] * m[2][3]  -  m[3][0] * m[0][1] * m[1][2] * m[2][3]  +  m[0][0] * m[3][1] * m[1][2] * m[2][3] +
            m[1][0] * m[0][1] * m[3][2] * m[2][3]  -  m[0][0] * m[1][1] * m[3][2] * m[2][3]  -  m[2][0] * m[1][1] * m[0][2] * m[3][3]  +  m[1][0] * m[2][1] * m[0][2] * m[3][3] +
            m[2][0] * m[0][1] * m[1][2] * m[3][3]  -  m[0][0] * m[2][1] * m[1][2] * m[3][3]  -  m[1][0] * m[0][1] * m[2][2] * m[3][3]  +  m[0][0] * m[1][1] * m[2][2] * m[3][3];

    }

    void Matrix4::inverse() {

        Matrix4 result;
        float d = determinant();
        if( d == 0 ) return;
        d = 1.0f / d;

        result.m[0][0] = d * (m[2][1] * m[3][2] * m[1][3]  -  m[3][1] * m[2][2] * m[1][3]  +  m[3][1] * m[1][2] * m[2][3]  -  m[1][1] * m[3][2] * m[2][3]  -  m[2][1] * m[1][2] * m[3][3]  +  m[1][1] * m[2][2] * m[3][3]);
        result.m[1][0] = d * (m[3][0] * m[2][2] * m[1][3]  -  m[2][0] * m[3][2] * m[1][3]  -  m[3][0] * m[1][2] * m[2][3]  +  m[1][0] * m[3][2] * m[2][3]  +  m[2][0] * m[1][2] * m[3][3]  -  m[1][0] * m[2][2] * m[3][3]);
        result.m[2][0] = d * (m[2][0] * m[3][1] * m[1][3]  -  m[3][0] * m[2][1] * m[1][3]  +  m[3][0] * m[1][1] * m[2][3]  -  m[1][0] * m[3][1] * m[2][3]  -  m[2][0] * m[1][1] * m[3][3]  +  m[1][0] * m[2][1] * m[3][3]);
        result.m[3][0] = d * (m[3][0] * m[2][1] * m[1][2]  -  m[2][0] * m[3][1] * m[1][2]  -  m[3][0] * m[1][1] * m[2][2]  +  m[1][0] * m[3][1] * m[2][2]  +  m[2][0] * m[1][1] * m[3][2]  -  m[1][0] * m[2][1] * m[3][2]);
        result.m[0][1] = d * (m[3][1] * m[2][2] * m[0][3]  -  m[2][1] * m[3][2] * m[0][3]  -  m[3][1] * m[0][2] * m[2][3]  +  m[0][1] * m[3][2] * m[2][3]  +  m[2][1] * m[0][2] * m[3][3]  -  m[0][1] * m[2][2] * m[3][3]);
        result.m[1][1] = d * (m[2][0] * m[3][2] * m[0][3]  -  m[3][0] * m[2][2] * m[0][3]  +  m[3][0] * m[0][2] * m[2][3]  -  m[0][0] * m[3][2] * m[2][3]  -  m[2][0] * m[0][2] * m[3][3]  +  m[0][0] * m[2][2] * m[3][3]);
        result.m[2][1] = d * (m[3][0] * m[2][1] * m[0][3]  -  m[2][0] * m[3][1] * m[0][3]  -  m[3][0] * m[0][1] * m[2][3]  +  m[0][0] * m[3][1] * m[2][3]  +  m[2][0] * m[0][1] * m[3][3]  -  m[0][0] * m[2][1] * m[3][3]);
        result.m[3][1] = d * (m[2][0] * m[3][1] * m[0][2]  -  m[3][0] * m[2][1] * m[0][2]  +  m[3][0] * m[0][1] * m[2][2]  -  m[0][0] * m[3][1] * m[2][2]  -  m[2][0] * m[0][1] * m[3][2]  +  m[0][0] * m[2][1] * m[3][2]);
        result.m[0][2] = d * (m[1][1] * m[3][2] * m[0][3]  -  m[3][1] * m[1][2] * m[0][3]  +  m[3][1] * m[0][2] * m[1][3]  -  m[0][1] * m[3][2] * m[1][3]  -  m[1][1] * m[0][2] * m[3][3]  +  m[0][1] * m[1][2] * m[3][3]);
        result.m[1][2] = d * (m[3][0] * m[1][2] * m[0][3]  -  m[1][0] * m[3][2] * m[0][3]  -  m[3][0] * m[0][2] * m[1][3]  +  m[0][0] * m[3][2] * m[1][3]  +  m[1][0] * m[0][2] * m[3][3]  -  m[0][0] * m[1][2] * m[3][3]);
        result.m[2][2] = d * (m[1][0] * m[3][1] * m[0][3]  -  m[3][0] * m[1][1] * m[0][3]  +  m[3][0] * m[0][1] * m[1][3]  -  m[0][0] * m[3][1] * m[1][3]  -  m[1][0] * m[0][1] * m[3][3]  +  m[0][0] * m[1][1] * m[3][3]);
        result.m[3][2] = d * (m[3][0] * m[1][1] * m[0][2]  -  m[1][0] * m[3][1] * m[0][2]  -  m[3][0] * m[0][1] * m[1][2]  +  m[0][0] * m[3][1] * m[1][2]  +  m[1][0] * m[0][1] * m[3][2]  -  m[0][0] * m[1][1] * m[3][2]);
        result.m[0][3] = d * (m[2][1] * m[1][2] * m[0][3]  -  m[1][1] * m[2][2] * m[0][3]  -  m[2][1] * m[0][2] * m[1][3]  +  m[0][1] * m[2][2] * m[1][3]  +  m[1][1] * m[0][2] * m[2][3]  -  m[0][1] * m[1][2] * m[2][3]);
        result.m[1][3] = d * (m[1][0] * m[2][2] * m[0][3]  -  m[2][0] * m[1][2] * m[0][3]  +  m[2][0] * m[0][2] * m[1][3]  -  m[0][0] * m[2][2] * m[1][3]  -  m[1][0] * m[0][2] * m[2][3]  +  m[0][0] * m[1][2] * m[2][3]);
        result.m[2][3] = d * (m[2][0] * m[1][1] * m[0][3]  -  m[1][0] * m[2][1] * m[0][3]  -  m[2][0] * m[0][1] * m[1][3]  +  m[0][0] * m[2][1] * m[1][3]  +  m[1][0] * m[0][1] * m[2][3]  -  m[0][0] * m[1][1] * m[2][3]);
        result.m[3][3] = d * (m[1][0] * m[2][1] * m[0][2]  -  m[2][0] * m[1][1] * m[0][2]  +  m[2][0] * m[0][1] * m[1][2]  -  m[0][0] * m[2][1] * m[1][2]  -  m[1][0] * m[0][1] * m[2][2]  +  m[0][0] * m[1][1]*m[2][2]);

        *this = result;

    }

    void Matrix4::setIdentity() {
        this->m[0][0] = 1;
        this->m[1][1] = 1;
        this->m[2][2] = 1;
        this->m[3][3] = 1;
    }

    Matrix4 operator +(const Matrix4& lhs, const Matrix4& rhs) {

        return Matrix4(
            lhs[0] + rhs[0],
            lhs[1] + rhs[1],
            lhs[2] + rhs[2],
            lhs[3] + rhs[3]
        );
    }

    Matrix4 operator +(float scalar, const Matrix4& rhs) {

        return Matrix4(
            scalar + rhs[0],
            scalar + rhs[1],
            scalar + rhs[2],
            scalar + rhs[3]
        );
    }

    Matrix4 operator +(const Matrix4& lhs, float scalar) {

        return Matrix4(
            lhs[0] + scalar,
            lhs[1] + scalar,
            lhs[2] + scalar,
            lhs[3] + scalar
        );
    }

    Matrix4 operator -(const Matrix4& lhs, const Matrix4& rhs) {

        return Matrix4(
            lhs[0] - rhs[0],
            lhs[1] - rhs[1],
            lhs[2] - rhs[2],
            lhs[3] - rhs[3]
        );
    }

    Matrix4 operator -(float scalar, const Matrix4& rhs) {

        return Matrix4(
            scalar - rhs[0],
            scalar - rhs[1],
            scalar - rhs[2],
            scalar - rhs[3]
        );
    }

    Matrix4 operator -(const Matrix4& lhs, float scalar) {

        return Matrix4(
            lhs[0] - scalar,
            lhs[1] - scalar,
            lhs[2] - scalar,
            lhs[3] - scalar
        );
    }

    Matrix4 operator *(const Matrix4& lhs, float scalar) {

        return Matrix4(
            lhs[0] * scalar,
            lhs[1] * scalar,
            lhs[2] * scalar,
            lhs[3] * scalar
        );
    }

    Matrix4 operator *(float scalar, const Matrix4& rhs) {

        return Matrix4(
            scalar * rhs[0],
            scalar * rhs[1],
            scalar * rhs[2],
            scalar * rhs[3]
        );
    }

    Matrix4 operator *(const Matrix4& lhs, const Matrix4& rhs) {

//        Matrix4 result;
//        result[0][0] = lhs[0][0] * rhs[0][0] + lhs[1][0] * rhs[0][1] + lhs[2][0] * rhs[0][2] + lhs[3][0] * rhs[0][3];
//        result[1][0] = lhs[0][0] * rhs[1][0] + lhs[1][0] * rhs[1][1] + lhs[2][0] * rhs[1][2] + lhs[3][0] * rhs[1][3];
//        result[2][0] = lhs[0][0] * rhs[2][0] + lhs[1][0] * rhs[2][1] + lhs[2][0] * rhs[2][2] + lhs[3][0] * rhs[2][3];
//        result[3][0] = lhs[0][0] * rhs[3][0] + lhs[1][0] * rhs[3][1] + lhs[2][0] * rhs[3][2] + lhs[3][0] * rhs[3][3];
//
//        result[0][1] = lhs[0][1] * rhs[0][0] + lhs[1][1] * rhs[0][1] + lhs[2][1] * rhs[0][2] + lhs[3][1] * rhs[0][3];
//        result[1][1] = lhs[0][1] * rhs[1][0] + lhs[1][1] * rhs[1][1] + lhs[2][1] * rhs[1][2] + lhs[3][1] * rhs[1][3];
//        result[2][1] = lhs[0][1] * rhs[2][0] + lhs[1][1] * rhs[2][1] + lhs[2][1] * rhs[2][2] + lhs[3][1] * rhs[2][3];
//        result[3][1] = lhs[0][1] * rhs[3][0] + lhs[1][1] * rhs[3][1] + lhs[2][1] * rhs[3][2] + lhs[3][1] * rhs[3][3];
//
//        result[0][2] = lhs[0][2] * rhs[0][0] + lhs[1][2] * rhs[0][1] + lhs[2][2] * rhs[0][2] + lhs[3][2] * rhs[0][3];
//        result[1][2] = lhs[0][2] * rhs[1][0] + lhs[1][2] * rhs[1][1] + lhs[2][2] * rhs[1][2] + lhs[3][2] * rhs[1][3];
//        result[2][2] = lhs[0][2] * rhs[2][0] + lhs[1][2] * rhs[2][1] + lhs[2][2] * rhs[2][2] + lhs[3][2] * rhs[2][3];
//        result[3][2] = lhs[0][2] * rhs[3][0] + lhs[1][2] * rhs[3][1] + lhs[2][2] * rhs[3][2] + lhs[3][2] * rhs[3][3];
//
//        result[0][3] = lhs[0][3] * rhs[0][0] + lhs[1][3] * rhs[0][1] + lhs[2][3] * rhs[0][2] + lhs[3][3] * rhs[0][3];
//        result[1][3] = lhs[0][3] * rhs[1][0] + lhs[1][3] * rhs[1][1] + lhs[2][3] * rhs[1][2] + lhs[3][3] * rhs[1][3];
//        result[2][3] = lhs[0][3] * rhs[2][0] + lhs[1][3] * rhs[2][1] + lhs[2][3] * rhs[2][2] + lhs[3][3] * rhs[2][3];
//        result[3][3] = lhs[0][3] * rhs[3][0] + lhs[1][3] * rhs[3][1] + lhs[2][3] * rhs[3][2] + lhs[3][3] * rhs[3][3];
//        return result;

        typename Matrix4::col_major const SrcA0 = lhs[0];
        typename Matrix4::col_major const SrcA1 = lhs[1];
        typename Matrix4::col_major const SrcA2 = lhs[2];
        typename Matrix4::col_major const SrcA3 = lhs[3];

        typename Matrix4::col_major const SrcB0 = rhs[0];
        typename Matrix4::col_major const SrcB1 = rhs[1];
        typename Matrix4::col_major const SrcB2 = rhs[2];
        typename Matrix4::col_major const SrcB3 = rhs[3];

        Matrix4 Result;
        Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
        Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
        Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
        Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
        return Result;

    }

    Matrix4 operator /(const Matrix4& lhs, float scalar) {

        return Matrix4(
            lhs[0] / scalar,
            lhs[1] / scalar,
            lhs[2] / scalar,
            lhs[3] / scalar
        );

    }

    Matrix4 operator /(float scalar, const Matrix4& rhs) {

        return Matrix4(
            scalar / rhs[0],
            scalar / rhs[1],
            scalar / rhs[2],
            scalar / rhs[3]
        );
    }

    Matrix4 operator /(const Matrix4& lhs, const Matrix4& rhs) {

        Matrix4 result(lhs);
        return result /= rhs;
    }

    bool operator ==(const Matrix4& lhs, const Matrix4& rhs) {

        return (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]) && (lhs[2] == rhs[2]) && (lhs[3] == rhs[3]);
    }

    bool operator !=(const Matrix4& lhs, const Matrix4& rhs) {

        return (lhs[0] != rhs[0]) || (lhs[1] != rhs[1]) || (lhs[2] != rhs[2]) || (lhs[3] != rhs[3]);
    }



}


