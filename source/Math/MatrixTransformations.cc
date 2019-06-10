/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include "MatrixTransformations.hh"

namespace ost {

    namespace Math {

        Matrix4 ortho(const float left, const float right,
                      const float bottom, const float top)
        {
            Matrix4 Result(1);
            Result[0][0] = (2) / (right - left);
            Result[1][1] = (2) / (top - bottom);
            Result[2][2] = - (1);
            Result[3][0] = - (right + left) / (right - left);
            Result[3][1] = - (top + bottom) / (top - bottom);
            return Result;
        }

        Matrix4 ortho(const float left, const float right, const float bottom,
                      const float top, const float zNear, const float zFar)
        {
            Matrix4 Result(1.0f);
            Result[0][0] = (2) / (right - left);
            Result[1][1] = (2) / (top - bottom);
            Result[2][2] = - (2) / (zFar - zNear);
            Result[3][0] = - (right + left) / (right - left);
            Result[3][1] = - (top + bottom) / (top - bottom);
            Result[3][2] = - (zFar + zNear) / (zFar - zNear);
            return Result;
        }

        Matrix4 scale(const Matrix4& lhs, const Vec3& rhs)
        {

            Matrix4 Result;
            Result[0] = lhs[0] * rhs[0];
            Result[1] = lhs[1] * rhs[1];
            Result[2] = lhs[2] * rhs[2];
            Result[3] = lhs[3];
            return Result;
        }

        Matrix4 translate(const Matrix4& lhs, const Vec3& rhs)
        {
           Matrix4 Result(lhs);
            Result[3] = lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3];
            return Result;
        }

        Matrix4 rotate(const Matrix4& lhs, const Vec3& rhs, const float angle)
        {
            float const a = angle;
            float const c = static_cast<float>(cos(a));
            float const s = static_cast<float>(sin(a));

            Vec3 axis(rhs.normalized());
            Vec3 temp(((1) - c) * axis);

            Matrix4 Rotate;
            Rotate[0][0] = c + temp[0] * axis[0];
            Rotate[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
            Rotate[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

            Rotate[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
            Rotate[1][1] = c + temp[1] * axis[1];
            Rotate[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

            Rotate[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
            Rotate[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
            Rotate[2][2] = c + temp[2] * axis[2];

            Matrix4 Result;
            Result[0] = lhs[0] * Rotate[0][0] + lhs[1] * Rotate[0][1] + lhs[2] * Rotate[0][2];
            Result[1] = lhs[0] * Rotate[1][0] + lhs[1] * Rotate[1][1] + lhs[2] * Rotate[1][2];
            Result[2] = lhs[0] * Rotate[2][0] + lhs[1] * Rotate[2][1] + lhs[2] * Rotate[2][2];
            Result[3] = lhs[3];
            return Result;
        }
    }
}