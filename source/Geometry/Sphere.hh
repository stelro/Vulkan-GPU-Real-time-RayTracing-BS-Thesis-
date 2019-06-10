//
// Created by stel on 2/7/2018.
//

#ifndef STRACER_SPHERE_HH
#define STRACER_SPHERE_HH

#include "../core.hh"
#include "../Math/Vector.hh"
#include "../Materials.hh"
#include "../json/json.hpp"

namespace ost {

  struct Sphere {

    Sphere()
      : center(0.0f)
      , diffuse(1.0f)
      , radius(0.0f)
      , material(0)
      , object_id(0)
      , ref_index(0.0f)
      , fuzz(0.2f)
{ }

    Sphere(const Vec3& t_center, const Vec3& t_diffuse,
      float t_radius, int t_material,int t_ref_index, float t_fuzz)
      : center(t_center)
      , diffuse(t_diffuse)
      , radius(t_radius)
      , material(t_material)
      , ref_index(t_ref_index)
      , fuzz(t_fuzz) { }

    alignas(16)   ost::Vec3     center;
    alignas(16)   ost::Vec3     diffuse;
                  float         radius;
                  int           material;
                  u32           object_id;
                  float         ref_index;
                  float         fuzz;
  };

  [[maybe_unused]]
  OST_FORCE_INLINE
  Sphere new_sphere(const ost::Vec3 &t_center, float t_radius, const ost::Vec3 &diffuse, Material material, float ref_index = 1.0f, float fuzz = 0.2f) noexcept {
    Sphere sphere;
    sphere.center = t_center;
    sphere.radius = t_radius;
    sphere.diffuse = diffuse;
    sphere.material = static_cast<int>(material);
    sphere.ref_index = ref_index;
    sphere.fuzz = fuzz;
    return sphere;
  }
}

#endif //STRACER_SPHERE_HH
