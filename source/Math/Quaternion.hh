/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

/*
 * Some parts are taken from Lumix Engine ( Open Source )
 * All credits goes to Lumix Engine
 */

//Quaternion used mostly for complex rotations

#ifndef PROJECT_QUATERNION_HPP
#define PROJECT_QUATERNION_HPP

//Engine Internal
#include "Vector.hh"

namespace ost {

    class Matrix4;

    struct Quaternion {

        struct AxisAngle {
            Vec3 axis;
            float angle;
        };

        Quaternion()
            : x(0.0f)
            , y(0.0f)
            , z(0.0f)
            , w(0.0f) { }

        Quaternion(float a, float b, float c, float d)
            : x(a)
            , y(b)
            , z(c)
            , w(d) { }

        Quaternion(const Vec3& axis, float angle);
        void fromEuler(const Vec3& euler);
        Vec3 toEuler() const;
        AxisAngle getAxisAngle() const;
        void conjugate();
        Quaternion conjugated() const;
        void normilize();
        Matrix4 toMatrix() const;
        Vec3 rotate(const Vec3& vec) const;
        Quaternion operator *(const Quaternion& rhs) const;
        Quaternion operator -() const;

        static Quaternion vec3ToVec3(const Vec3& lhs, const Vec3& rhs);

        void set(float a, float b, float c, float d) {
            x = a;
            y = b;
            z = c;
            w = d;
        }

        float               x;
        float               y;
        float               z;
        float               w;

    };

    void nlerp(const Quaternion& q1, const Quaternion& q2, Quaternion * out, float t);
}


#endif //PROJECT_QUATERNION_HPP
