/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#ifndef PROJECT_MATRIXNEW_HPP
#define PROJECT_MATRIXNEW_HPP

//Engine Internal
#include "Vector.hh"
#include "Quaternion.hh"

//OpenGL Includes
#include <glm/glm.hpp>

namespace ost {

    class Matrix4 {

        typedef Vec4 col_major;
        typedef Vec4 row_major;

    private:
        Matrix4 compute_inverse(const Matrix4& rhs);
        col_major m[4];
    public:

        /* Constructors */

        Matrix4();
        Matrix4(const Matrix4& rhs);
        Matrix4(float val);

        Matrix4(
            const col_major& v1,
            const col_major& v2,
            const col_major& v3,
            const col_major& v4
        );

        Matrix4(
            float x0, float y0, float z0, float w0,
            float x1, float y1, float z1, float w1,
            float x2, float y2, float z2, float w2,
            float x3, float y3, float z3, float w3
        );

        typename Matrix4::col_major& operator [](size_t index);
        typename Matrix4::col_major const& operator [](size_t index) const;

        explicit operator glm::mat4();

        /* Unary arithmetic operations */

        Matrix4& operator =(const Matrix4& rhs);
        Matrix4& operator +=(float rhs);
        Matrix4& operator +=(const Matrix4& rhs);
        Matrix4& operator -=(float rhs);
        Matrix4& operator -=(const Matrix4& rhs);
        Matrix4& operator *=(float rhs);
        Matrix4& operator *=(const Matrix4& rhs);
        Matrix4& operator /=(float rhs);
        Matrix4& operator /=(const Matrix4& rhs);

        /* Increment and decrement operators */

        Matrix4& operator ++();
        Matrix4& operator --();
        const Matrix4 operator ++(int);
        const Matrix4 operator --(int);

        /* Unary operators */

        Matrix4 operator +(const Matrix4& rhs);
        Matrix4 operator -(const Matrix4& rhs);

        /* Binary operators friends*/

        friend Matrix4 operator +(const Matrix4& lhs, const Matrix4& rhs);
        friend Matrix4 operator +(float val, const Matrix4& rhs);
        friend Matrix4 operator +(const Matrix4& lhs, float val);
        friend Matrix4 operator -(const Matrix4& lhs, const Matrix4& rhs);
        friend Matrix4 operator -(float val, const Matrix4& rhs);
        friend Matrix4 operator -(const Matrix4& lhs, float val);
        friend Matrix4 operator *(const Matrix4& lhs, float val);
        friend Matrix4 operator *(float s, const Matrix4& rhs);
        friend Matrix4 operator *(const Matrix4& lhs, const Matrix4& rhs);
        friend Matrix4 operator /(const Matrix4& lhs, float val);
        friend Matrix4 operator /(float val, const Matrix4& rhs);
        friend Matrix4 operator /(const Matrix4& lhs, const Matrix4& rhs);

        /* Boolean Operators */

        friend bool operator ==(const Matrix4& lhs, const Matrix4& rhs);
        friend bool operator !=(const Matrix4& lhs, const Matrix4& rhs);

        Vec3 getZVector() const;
        Vec3 getYVector() const;
        Vec3 getXVector() const;
        void setXVector(const Vec3& v);
        void setYVector(const Vec3& v);
        void setZVector(const Vec3& v);


        float determinant() const;
        void inverse();
        void setIdentity();

    };

    Matrix4 operator +(const Matrix4& lhs, const Matrix4& rhs);
    Matrix4 operator +(float val, const Matrix4& rhs);
    Matrix4 operator +(const Matrix4& lhs, float val);
    Matrix4 operator -(const Matrix4& lhs, const Matrix4& rhs);
    Matrix4 operator -(float val, const Matrix4& rhs);
    Matrix4 operator -(const Matrix4& lhs, float val);
    Matrix4 operator *(const Matrix4& lhs, float val);
    Matrix4 operator *(float s, const Matrix4& rhs);
    Matrix4 operator *(const Matrix4& lhs, const Matrix4& rhs);
    Matrix4 operator /(const Matrix4& lhs, float val);
    Matrix4 operator /(float val, const Matrix4& rhs);
    Matrix4 operator /(const Matrix4& lhs, const Matrix4& rhs);
    bool operator ==(const Matrix4& lhs, const Matrix4& rhs);
    bool operator !=(const Matrix4& lhs, const Matrix4& rhs);

}



#endif //PROJECT_MATRIXNEW_HPP
