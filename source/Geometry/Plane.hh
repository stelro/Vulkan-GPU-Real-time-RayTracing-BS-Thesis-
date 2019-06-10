//
// Created by stel on 24/1/2019.
//

#ifndef STRACER_PLANE_HH
#define STRACER_PLANE_HH

#include "../core.hh"
#include "../Math/Vector.hh"
#include "../Materials.hh"

namespace ost {

  struct Plane {
    alignas(16)     ost::Vec3         normal;
    alignas(16)     ost::Vec3         diffuse;
                    float             distance;
                    int               material;
                    int               object_id;
                    float             ref_index;
                    float             fuzz;
  };

  [[maybe_unused]]
  OST_FORCE_INLINE
  Plane new_plane(const ost::Vec3 &t_normal, float distance, const ost::Vec3 &diffuse, Material material, float ref_index = 1.0f, float fuzz = 0.2f) noexcept {
    Plane plane;
    plane.distance = distance;
    plane.normal = t_normal;
    plane.diffuse = diffuse;
    plane.material = static_cast<int>(material);
    plane.ref_index = ref_index;
    plane.fuzz = fuzz;
    return plane;
  }

}

#endif //STRACER_PLANE_HH
