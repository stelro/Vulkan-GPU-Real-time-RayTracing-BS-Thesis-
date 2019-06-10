/* =======================================================================
   $File: Scene.hh
   $Date: 22/2/2019	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


#pragma once

#include "json.hpp"

//Engine Includes
#include "../Math/Vector.hh"
#include "../Geometry/Sphere.hh"
#include "../Geometry/Plane.hh"
#include "../Geometry/Rectangle.hh"
#include "../Geometry/Box.hh"
#include "../core.hh"

//C++ Includes
#include <fstream>
#include <string_view>
#include <vector>
#include <array>

namespace ost {

    namespace ns {
      using nlohmann::json;
    }


    class Scene {
    public:

      explicit Scene(const std::string& filename) noexcept;
      ~Scene() noexcept;

      OST_DISABLE_COPY(Scene)
      OST_DISABLE_MOVE(Scene)

      void constructSpheres() noexcept;
      void constructPlanes() noexcept;
      void constructRectangles() noexcept;
      void constructBoxes() noexcept;

      std::vector<Sphere> getSpheres() const noexcept;
      std::vector<Plane> getPlanes() const noexcept;
      std::vector<Rectangle> getRectangles() const noexcept;
      std::vector<Box> getBoxes() const noexcept;

      float getCameraFov() const noexcept;

      std::array<float, 3> getCameraOrigin() const noexcept;
      std::array<float, 3> getCameraLookAt() const noexcept;
      ost::Vec3 getAmbientColor() const noexcept;

      u32 getWidth() const noexcept;
      u32 getHeight() const noexcept;
      u32 getTotalObjects() const noexcept;

      bool spheresAreEnabled() const noexcept;
      bool rectsAreEnabled() const noexcept;
      bool planesAreEnabled() const noexcept;
      bool boxesAreEnabled() const noexcept;

    private:

      ns::json m_jsonString;

      std::vector<Sphere> m_spheres;
      std::vector<Plane> m_planes;
      std::vector<Rectangle> m_rectangles;
      std::vector<Box> m_boxes;

      u32 m_objectId;

    };

}

