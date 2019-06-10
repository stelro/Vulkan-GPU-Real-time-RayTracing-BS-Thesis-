//
// Created by stel on 27/1/2019.
//

#ifndef STRACER_RECTANGLE_HH
#define STRACER_RECTANGLE_HH

#include "../core.hh"
#include "../Math/Vector.hh"
#include "../Materials.hh"

namespace ost {

  enum class AABB_SIDE : int{
    XZ_SIDE = 0,
    XY_SIDE = 1,
    YZ_SIDE = 2,
  };

  struct Rectangle  {
    alignas(16) ost::Vec4     aabb;
    alignas(16) ost::Vec3     diffuse;
    float                     k;
    int                       side;
    int                       material;
    u32                       object_id;
    float                     ref_index;
    float                     fuzz;
    bool                      reverse_normals;
  };

  [[maybe_unused]]
  OST_FORCE_INLINE
  Rectangle new_Rectangle(const Vec4& aabb,const ost::Vec3 &diffuse, float k,
    AABB_SIDE side, Material material,float ref_index = 1.0f, float fuzz = 0.2f, bool reverse_norm = false) noexcept {

    Rectangle rect;
    rect.aabb = aabb;
    rect.diffuse = diffuse;
    rect.k = k;
    rect.side = static_cast<int>(side);
    rect.ref_index = ref_index;
    rect.reverse_normals = reverse_norm;
    rect.material = static_cast<int>(material);
    rect.fuzz = fuzz;
    return rect;

  }

}

#endif //STRACER_RECTANGLE_HH
