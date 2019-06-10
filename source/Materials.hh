/* =======================================================================
   $File: Materials.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#ifndef STRACER_MATERIALS_HH
#define STRACER_MATERIALS_HH

namespace ost {
  enum class Material : int {

    LAMBERTIAN  = 0,
    METAL       = 1,
    DIELECTRIC  = 2,
    EMITTER     = 3,

  };
}

#endif //STRACER_MATERIALS_HH
