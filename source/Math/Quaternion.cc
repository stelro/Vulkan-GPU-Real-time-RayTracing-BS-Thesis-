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
#include "Quaternion.hh"
#include "MathUtils.hh"

//C++ Includes
#include <cmath>
#include <cassert>


namespace ost {


    Quaternion::Quaternion(const Vec3 &axis, float angle)
    {
        float half_angle = angle * 0.5f;
        float s = sinf(half_angle);
        w = cosf(half_angle);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
    }

    void Quaternion::fromEuler(const Vec3 &euler)
    {
        assert(euler.x >= -Math::HALF_PI && euler.x <= Math::HALF_PI);
        float ex = euler.x * 0.5f;
        float ey = euler.y * 0.5f;
        float ez = euler.z * 0.5f;
        float sinX = sinf(ex);
        float cosX = cosf(ex);
        float sinY = sinf(ey);
        float cosY = cosf(ey);
        float sinZ = sinf(ez);
        float cosZ = cosf(ez);

        w = cosY * cosX * cosZ + sinY * sinX * sinZ;
        x = cosY * sinX * cosZ + sinY * cosX * sinZ;
        y = sinY * cosX * cosZ - cosY * sinX * sinZ;
        z = cosY * cosX * sinZ - sinY * sinX * cosZ;
    }

    Vec3 Quaternion::toEuler() const
    {
        // from urho3d
        float check = 2.0f * (-y * z + w * x);

        if (check < -0.995f)
        {
            return Vec3(
                -Math::PI * 0.5f, 0.0f, -atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)));
        }
        else if (check > 0.995f)
        {
            return Vec3(
                Math::PI * 0.5f, 0.0f, atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)));
        }
        else
        {
            return Vec3(asinf(check),
                        atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)),
                        atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)));
        }
    }

    Quaternion::AxisAngle Quaternion::getAxisAngle() const
    {
        return Quaternion::AxisAngle();

    }

    void Quaternion::conjugate()
    {
        w = -w;
    }

    Quaternion Quaternion::conjugated() const
    {
        return Quaternion(x, y, z, -w);
    }

    void Quaternion::normilize()
    {
        float l = 1/ static_cast<float> (sqrt(x*x + y*y + z*z + w*w));
        x *= l;
        y *= l;
        z *= l;
        w *= l;
    }

//    Matrix4 Quaternion::toMatrix() const
  //  {
//        float fx = x + x;
//        float fy = y + y;
//        float fz = z + z;
//        float fwx = fx*w;
//        float fwy = fy*w;
//        float fwz = fz*w;
//        float fxx = fx*x;
//        float fxy = fy*x;
//        float fxz = fz*x;
//        float fyy = fy*y;
//        float fyz = fz*y;
//        float fzz = fz*z;
//
//        Matrix4 mtx;
//        mtx.mat11 = 1.0f - (fyy + fzz);
//        mtx.mat21 = fxy - fwz;
//        mtx.mat31 = fxz + fwy;
//        mtx.mat12 = fxy + fwz;
//        mtx.mat22 = 1.0f - (fxx + fzz);
//        mtx.mat32 = fyz - fwx;
//        mtx.mat13 = fxz - fwy;
//        mtx.mat23 = fyz + fwx;
//        mtx.mat33 = 1.0f - (fxx + fyy);
//
//        mtx.mat41 = mtx.mat42 = mtx.mat43 = mtx.mat14 = mtx.mat24 = mtx.mat34 = 0;
//        mtx.mat44 = 1;
//        return mtx;
   // }

    Vec3 Quaternion::rotate(const Vec3 &vec) const
    {
        //nVidia SDK implementation

        Vec3 uv, uuv;
        Vec3 qvec(x,y,z);
        uv = crossProduct(qvec, vec);
        uuv = crossProduct(qvec, uv);
        uv *= (2.0f * w);
        uuv *= 2.0f;

        return vec + uv + uuv;
    }

    Quaternion Quaternion::operator*(const Quaternion &rhs) const
    {
        return Quaternion(w * rhs.x + rhs.w * x + y * rhs.z - rhs.y * z,
                    w * rhs.y + rhs.w * y + z * rhs.x - rhs.z * x,
                    w * rhs.z + rhs.w * z + x * rhs.y - rhs.x * y,
                    w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
    }

    Quaternion Quaternion::operator-() const
    {
        return Quaternion(x, y, z, -w);
    }

    Quaternion Quaternion::vec3ToVec3(const Vec3 &lhs, const Vec3 &rhs)
    {
        float angle = acosf(dotProduct(lhs, rhs));
        Vec3 normal = crossProduct(lhs, rhs);
        float normal_len = normal.length();
        return Quaternion(normal_len < 0.001f ? Vec3(0, 1, 0) : normal * (1 / normal_len), angle);
    }

    void nlerp(const Quaternion &q1, const Quaternion &q2, Quaternion *out, float t)
    {
        float inv = 1.0f - t;
        if (q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w < 0) t = -t;
        float ox = q1.x * inv + q2.x * t;
        float oy = q1.y * inv + q2.y * t;
        float oz = q1.z * inv + q2.z * t;
        float ow = q1.w * inv + q2.w * t;
        float l = 1 / static_cast<float>(sqrt(ox * ox + oy * oy + oz * oz + ow * ow));
        ox *= l;
        oy *= l;
        oz *= l;
        ow *= l;
        out->x = ox;
        out->y = oy;
        out->z = oz;
        out->w = ow;
    }
}



