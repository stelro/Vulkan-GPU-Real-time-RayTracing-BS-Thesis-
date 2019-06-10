/* =======================================================================
   $File: RayEngine.hh
   $Date: 3/6/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


#pragma once

// Engine Includes
#include "VulkanEngine.hh"
#include "Vulkan/VulkanTexture.hh"
#include "Math/Vector.hh"
// C++ Includes
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <array>
#include <memory>
// Vulkan
#include <vulkan/vulkan.h>

#define VERTEX_BUFFER_BIND_ID 0
#define ENABLE_VALIDATION true


namespace ost {

	class VulkanEngine;
	class GraphicsStage;
	class ComputeStage;
	class IOManager;
	class Scene;

	class RayEngine {
	private:
		ost::Texture                                            m_textureComputeTarget;

		VulkanEngine*                                           m_vulkanEngine                            = nullptr;
		GraphicsStage*                                          m_graphicsStage                           = nullptr;
		ComputeStage*                                           m_computeStage                            = nullptr;

		Scene*                                                  m_scene                                   = nullptr;

		// FPS Timer
		float                                                   m_FPSTimer                                = 0.0f;

		// Frame Counter to Display the FPS
		u32                                                     m_frameCounter                            = 0;

		// Width And Height varibles
		u32                                                     m_width                                   = 1400;
		u32                                                     m_height                                  = 700;

		float                                                   m_lastFPS                                 = 0.0f;

		// Last frame time measured using high performance timer
		float                                                   m_deltaTime                               = 1.0f;


		// Frame rate independent timer value clamped from [-1.0 ... 1.0 ]
		// Used mostly in animations, rotations, random pixel offsets etc..
		float                                                   m_helperTimer                             = 0.0f;

		//Multiplier for speeding up or slowing down the global timer
		float                                                   m_timerSpeed                              = 0.25f;

		// Some events may pause the engine
		bool                                                    m_paused                                  = false;

		IOManager*                                              m_iomanager                               = nullptr;


		float                                                   m_aaStrength                              = 0.10f;
		int                                                     m_samplesPerPixel                         = 8;
		int                                                     m_rayDepth                                = 8;
		float                                                   m_fov                                     = 40.0f;
		std::array<float, 3>                                    m_cameraOrigin                            = {2.0f, 2.0f, -8.0f};
		std::array<float, 3>                                    m_cameraLookAt                            = {2.0f, 2.0f, 0.0f};

	public:

		explicit																								RayEngine(const std::string& filename)     noexcept;
																														~RayEngine()                               noexcept;

																														OST_DISABLE_MOVE(RayEngine)
																														OST_DISABLE_COPY(RayEngine)

		// Prepare a texture target that is used to store compute shader calculations
		void                                                    prepareTextureTarget(ost::Texture *tex,
																																									uint32_t width,
																																									uint32_t height,
																																									VkFormat format)     noexcept;

		void                                                    setupDescriptorPool()                      noexcept;
		void                                                    prepare()                                  noexcept;
		void                                                    engineLoop()                               noexcept;
		void                                                    updateUIOverlay()                          noexcept;
	};

}



