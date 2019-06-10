/* =======================================================================
   $File: VulkanSwapChain.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#pragma once

//Engine Includes
#include "VulkanTools.hh"
#include "../core.hh"
//Vulkan Includes

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

//C++ Includes
#include <cstdlib>
#include <string>
#include <cstdio>
#include <vector>

namespace ost {

  typedef struct _SwapChainBuffers {
    VkImage         image;
    VkImageView     view;
  } SwapChainBuffer;

  class VulkanSwapChain {
  private:
    VkInstance                                m_instance;
    VkDevice                                  m_device;
    VkPhysicalDevice                          m_physicalDevice;
    VkSurfaceKHR                              m_surface;
  public:
    VkFormat                                  colorFormat;
    VkColorSpaceKHR                           colorSpace;

    // Handle to the current swap chain, required for recreation
    VkSwapchainKHR                            swapChain                 = VK_NULL_HANDLE;
    u32                                       imageCount;
    std::vector<VkImage>                      images;
    std::vector<SwapChainBuffer>              buffers;

    // Queue family index of the detected graphics and presenting device queue
    u32                                       queueNodeIndex            = UINT32_MAX;

    void initSurface(GLFWwindow *window) noexcept;

    // Set instance, physical and logical device to use for the swapchain and get all required function pointers
    void connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device) noexcept ;

    // Create the swapchain and get it's images with given width and height
    void create(u32 *width, u32 *height, bool vsync = false) noexcept;

    // Acquires the next image in the swap chain
    VkResult acquireNextImage(VkSemaphore presentCompleteSemaphore, u32 *imageIndex) noexcept;

    // Queue an image for presentation
    VkResult queuePresent(VkQueue queue, u32 imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE) noexcept;

    // Destroy and free Vulkan resources used for the swapchain
    void destroy() noexcept;

  };

}
