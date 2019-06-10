/* =======================================================================
   $File: StageBase.hh
   $Date: 17/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


#pragma once

#include <vulkan/vulkan.h>
#include <string_view>
#include <vector>

namespace ost {

	class VulkanEngine;

	class StageBase {
	public:
		explicit                                    StageBase(VulkanEngine* vulkanEngine)           noexcept;
		virtual                                     ~StageBase()                                    noexcept;
		virtual void                                initialize()                                    noexcept = 0;

		virtual void                                setupPipeline()                                 noexcept = 0;
		virtual void                                setupDescriptorSet()                            noexcept = 0;
		virtual void                                setupDescriptorSetLayout()                      noexcept = 0;
		virtual void                                buildCommandBuffers()                           noexcept = 0;
		virtual void                                prepareStorageBuffers()                         noexcept = 0;
		virtual void                                prepareUniformBuffers()                         noexcept = 0;
		virtual void                                update(float dt)                                noexcept = 0;
		virtual void                                render(float dt)                                noexcept = 0;

	protected:

		/*
		 * Singleton Instance of VulkanEngine class
		 */
		VulkanEngine*                               m_VulkanEngine;

	};

}

