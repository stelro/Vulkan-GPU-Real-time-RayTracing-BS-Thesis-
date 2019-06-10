//
// Created by stel on 27/1/2019.
//

#ifndef STRACER_BOX_HH
#define STRACER_BOX_HH

#include "../core.hh"
#include "../Math/Vector.hh"
#include "../Materials.hh"

namespace ost {


  struct Box  {
    alignas(16) ost::Vec3     vmin;
    alignas(16) ost::Vec3     vmax;
    alignas(16) ost::Vec3     diffuse;
    int                       material;
    u32                       object_id;
    float                     ref_index;
    float                     fuzz;
  };

  [[maybe_unused]]
  OST_FORCE_INLINE
  Box new_Box(const Vec3& vmin, const Vec3& vmax,const ost::Vec3 &diffuse,
     Material material,float ref_index = 1.0f, float fuzz = 0.2f) noexcept {

    Box box;
    box.vmin = vmin;
    box.vmax = vmax;
    box.diffuse = diffuse;
    box.material = static_cast<int>(material);
    box.ref_index = ref_index;
    box.fuzz = fuzz;
    return box;

  }

}

#endif //STRACER_BOX_HH
