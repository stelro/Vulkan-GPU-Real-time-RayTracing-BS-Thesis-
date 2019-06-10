/* =======================================================================
   $File: VulkanEngine.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <sys/stat.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <string>
#include <array>
#include <numeric>

#include "Vulkan/VulkanTools.hh"

#include "Vulkan/VulkanInitializers.hh"
#include "Vulkan/VulkanDevice.hh"
#include "Vulkan/VulkanSwapChain.hh"
#include "Vulkan/VulkanUIOverlay.hh"

#include "core.hh"

namespace ost {

	class VulkanEngine {
	private:

		// Destination dimensions for resizing the window
		bool resizing = false;

		// Called if the window is resized and some resources have to be recreated
		void windowResize()                                                                                                                 noexcept ;

		explicit VulkanEngine(bool enableValidation, u32 width, u32 height)                                                                 noexcept ;

		~VulkanEngine()                                                                                                                     noexcept;

		// Singleton instance
		static VulkanEngine*                                    m_VulkanEngineInstance;

		// Vulkan instance, stores all per-application states
		VkInstance                                              m_instance;

		// GLFWWindow, window where the generated texture will be displayed
		GLFWwindow*                                             m_window;

		// Physical device (GPU) that Vulkan will use
		VkPhysicalDevice                                        m_physicalDevice;

		// Stores physical device properties (for e.g. checking device limits)
		VkPhysicalDeviceProperties                              m_deviceProperties;

		// Stores the features available on the selected physical device (for e.g. checking if a feature is available)
		VkPhysicalDeviceFeatures                                m_deviceFeatures;

		// Stores all available memory (type) properties for the physical device
		VkPhysicalDeviceMemoryProperties                        m_deviceMemoryProperties;

		// Logical device, application's view of the physical device (GPU), it's basically a handle to physical device
		VkDevice                                                m_device;

		// Handle to the device graphics queue that command buffers are submitted to
		VkQueue                                                 m_gpuQueue;

		// Depth buffer format (selected during Vulkan initialization)
		VkFormat                                                m_depthFormat;

		// Command buffer pool
		VkCommandPool                                           m_commandPool;

		// Pipeline stages used to wait at for graphics queue submissions
		VkPipelineStageFlags                                    m_submitPipelineStages;

		// Descriptor set pool
		VkDescriptorPool                                        m_descriptorPool;

		// Contains command buffers and semaphores to be presented to the queue
		VkSubmitInfo                                            m_submitInfo;

		// Global render pass for frame buffer writes
		VkRenderPass                                            m_renderPass;

		// Active frame buffer index
		u32                                                     m_currentBuffer;

		// Pipeline cache object
		VkPipelineCache                                         m_pipelineCache;

		// Set to true when all the vulkan systems are initialized and ready to render frame
		bool                                                    m_prepared;

		// GLFW Window width
		u32                                                     m_width;

		// GLFW Window height
		u32                                                     m_height;

		// Defines a frame rate independent timer value clamped from -1.0...1.0
		// For use in animations, rotations, etc.
		float                                                   m_timer;

		// Multiplier for speeding up (or slowing down) the global timer
		float                                                   m_timerSpeed;

		bool                                                    m_paused;

		// Use to adjust mouse zoom speed
		float                                                   m_zoomSpeed;

		std::string                                             m_windowTitle;
		std::string                                             m_engineName;

		UIOverlay*                                              m_UIOverlay;

		// Encapsulates vulkan physical and logical device
		ost::VulkanDevice*                                      m_vulkanDevice;

		// Wraps the swap chain to present images (frame buffers) to the windowing system
		ost::VulkanSwapChain                                    m_swapChain;

		// Command buffers used for rendering
		std::vector<VkCommandBuffer>                            m_drawCmdBuffers;

		// List of available frame buffers (same as number of swap chain images)
		std::vector<VkFramebuffer>                              m_frameBuffers;

		/**
		* Set of physical device features to be enabled
		*
		* @note By default no physical device features are enabled
		*/
		VkPhysicalDeviceFeatures                                m_enabledFeatures;

		// Set of device extensions to be enabled
		std::vector<const char *>                               m_enabledDeviceExtensions;
		std::vector<const char *>                               m_enabledInstanceExtensions;

		std::vector<VkShaderModule>                             m_shaderModules;

		// Synchronization semaphores
		struct {
			// Swap chain image presentation
			VkSemaphore presentComplete;
			// Command buffer submission and execution
			VkSemaphore renderComplete;
			// UI overlay submission and execution
			VkSemaphore overlayComplete;
		} m_semaphores;

		// Raytracing engine settings that can be changed e.g. by command line arguments
		struct Settings {
			// Activates validation layers (and message output) when set to true
			bool validation = true;
			// Set to true if fullscreen mode has been requested via command line
			bool fullscreen = false;
			// Set to true if v-sync will be forced for the swapchain
			bool vsync = true;
			// Enable UI overlay
			bool overlay = false;
		} m_settings;

		struct {
			VkImage image;
			VkDeviceMemory mem;
			VkImageView view;
		} m_depthStencil;

		static std::vector<const char *>                        m_commandLineArgs;


	public:

		static VulkanEngine*                                    create(bool enableValidation, u32 width, u32 height)                        noexcept ;
		static VulkanEngine*                                    getInstance()                                                               noexcept ;

		OST_DISABLE_MOVE(VulkanEngine)
		OST_DISABLE_COPY(VulkanEngine)

		static void                                             destroy()                                                                   noexcept ;
		VkPhysicalDevice                                        getPhysicalDevice()                                                         noexcept ;
		VkPipelineShaderStageCreateInfo                         loadShader(std::string_view fileName,
		                                                                    VkShaderStageFlagBits stage)                                    noexcept ;

		VkDevice                                                getDevice()                                                                 noexcept ;
		VkQueue                                                 getGpuQueue()                                                               noexcept ;
		VkCommandPool                                           getCommandPool()                                                            noexcept ;
		std::vector<VkCommandBuffer>                            getDrawCmdBuffers()                                                         const noexcept ;
		void                                                    setDrawCmdBuffers(const std::vector<VkCommandBuffer>& drawCmdBuffers)       noexcept ;
		VkRenderPass                                            getRenderPass()                                                             noexcept ;
		std::vector<VkFramebuffer>                              getFrameBuffers()                                                           const noexcept ;
		void                                                    setFrameBuffers(const std::vector<VkFramebuffer>& frameBuffers)             noexcept ;
		VkDescriptorPool*                                       getDescriptorPool()                                                         noexcept ;
		VkPipelineCache                                         getPipelineCache()                                                          noexcept ;
		u32                                                     getWidth()                                                                  const noexcept ;
		void                                                    setWidth(u32 width)                                                         noexcept ;
		u32                                                     getHeight()                                                                 const noexcept ;
		void                                                    setHeight(u32 height)                                                       noexcept ;
		VulkanDevice&                                           getVulkanDevice()                                                           noexcept ;
		VkPhysicalDeviceProperties                              getDeviceProperties()                                                       noexcept ;


		// Setup the vulkan instance, enable required extensions and connect to the physical device (GPU)
		void                                                    initVulkan()                                                                noexcept ;

		// Init GLFW Window
		void                                                    initWindow()                                                                noexcept;


		// Create the application wide Vulkan instance
		VkResult                                                createVulkanInstance(bool enableValidation)                                 noexcept ;


		// Called when view change occurs
		// Containing view dependant matrices
		void                                                    viewChanged()                                                               noexcept ;

		// Called when the window has been resized
		void                                                    windowResized()                                                             noexcept ;

		// Called in case of an event where e.g. the framebuffer has to be rebuild and thus
		// all command buffers that may reference this
		void                                                    buildCommandBuffers()                                                       noexcept ;

		// Creates a new (graphics) command pool object storing command buffers
		void                                                    createCommandPool()                                                         noexcept ;

		// Setup default depth and stencil views
		void                                                    setupDepthStencil()                                                         noexcept ;

		// Create framebuffers for all requested swap chain images
		void                                                    setupFrameBuffer()                                                          noexcept ;

		// Setup a default render pass
		void                                                    setupRenderPass()                                                           noexcept ;

		//Called after the physical device features have been read, can be used to set features to enable on the device
		void                                                    getEnabledFeatures()                                                        noexcept ;

		// Connect and prepare the swap chain
		void                                                    initSwapchain()                                                             noexcept ;

		// Create swap chain images
		void                                                    setupSwapChain()                                                            noexcept ;

		// Check if command buffers are valid (!= VK_NULL_HANDLE)
		bool                                                    checkCommandBuffers()                                                       noexcept ;

		// Create command buffers for drawing commands
		void                                                    createCommandBuffers()                                                      noexcept ;

		// Destroy all command buffers and set their handles to VK_NULL_HANDLE
		// May be necessary during runtime if options are toggled
		void                                                    destroyCommandBuffers()                                                     noexcept;

		// Command buffer creation
		// Creates and returns a new command buffer
		VkCommandBuffer                                         createCommandBuffer(VkCommandBufferLevel level, bool begin)                 noexcept;

		// End the command buffer, submit it to the queue and free (if requested)
		// Note : Waits for the queue to become idle
		void                                                    flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free) noexcept;

		// Create a cache pool for rendering pipelines
		void                                                    createPipelineCache()                                                       noexcept;

		// Prepare commonly used Vulkan functions
		void                                                    prepare()                                                                   noexcept;

		// Prepare the frame for workload submission
		// - Acquires the next image from the swap chain
		// - Sets the default wait and signal semaphores
		void                                                    prepareFrame()                                                              noexcept;

		// Submit the frames' workload
		void                                                    submitFrame()                                                               noexcept;

		// Get the GLFW Window
		GLFWwindow*                                             getWindow()                                                                 noexcept ;

		void                                                    submitCmdBuffer()                                                           noexcept ;

		void                                                    engineIsPrepared(bool prep)                                                 noexcept ;

		bool                                                    isPrepared()                                                                const noexcept ;

		float                                                   getTimer()                                                                  const noexcept ;

		void                                                    setWindowTitle(std::string_view name)                                       noexcept ;

		UIOverlay*                                              getUIOverlay()                                                              noexcept ;

		bool                                                    vSycnEnabled()                                                              const;
		bool                                                    validationLayersEnabled()                                                   const;

	};

}


