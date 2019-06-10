/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#ifndef PROJECT_MATRIXTRANSFORMATIONS_HPP
#define PROJECT_MATRIXTRANSFORMATIONS_HPP

//Engine Internal
#include "Matrix.hh"

namespace ost {

    namespace Math {

        Matrix4 ortho(const float left, const float right,
                      const float bottom, const float top);

        Matrix4 ortho(const float left, const float right, const float bottom,
                      const float top, const float zNear, const float zFar);

        Matrix4 scale(const Matrix4& lhs, const Vec3& rhs);

        Matrix4 translate(const Matrix4& lhs, const Vec3& rhs);

        Matrix4 rotate(const Matrix4& lhs, const Vec3& rhs,const float angle);
    }
}


#endif //PROJECT_MATRIXTRANSFORMATIONS_HPP
