/* =======================================================================
   $File: StageBase.cc
   $Date: 17/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include "StageBase.hh"
#include "core.hh"
#include "VulkanEngine.hh"

namespace ost {

  StageBase::~StageBase() noexcept {


  }

  StageBase::StageBase(VulkanEngine* vulkanEngine) noexcept
    : m_VulkanEngine(vulkanEngine)
  {

  }

}