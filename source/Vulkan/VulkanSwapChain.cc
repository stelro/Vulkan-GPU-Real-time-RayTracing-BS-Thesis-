/* =======================================================================
   $File: VulkanSwapChain.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


#include "VulkanSwapChain.hh"
#include "../core.hh"

namespace ost {

  void VulkanSwapChain::initSurface(GLFWwindow *window) noexcept {

      VkResult err = glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface);

      if ( err != VK_SUCCESS ) {
          ost::exitFatal("Could not create surface!", err);
      }

      // Get available queue family properties
      u32 queueCount;
      vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, nullptr);
      OST_ASSERT(queueCount >= 1);

      std::vector<VkQueueFamilyProperties> queueProps(queueCount);
      vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, queueProps.data());

      // Iterate over each queue to learn whether it supports presenting:
      // Find a queue with present support
      // Will be used to present the swap chain images to the windowing system
      std::vector<VkBool32> supportsPresent(queueCount);
      for ( u32 i = 0; i < queueCount; i++ ) {
          vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &supportsPresent[i]);
      }

      // Search for a graphics and a present queue in the array of queue
      // families, try to find one that supports both
      u32 graphicsQueueNodeIndex = UINT32_MAX;
      u32 presentQueueNodeIndex = UINT32_MAX;

      for ( u32 i = 0; i < queueCount; i++ ) {
          if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 ) {
              if ( graphicsQueueNodeIndex == UINT32_MAX) {
                  graphicsQueueNodeIndex = i;
              }

              if ( supportsPresent[i] == VK_TRUE ) {
                  graphicsQueueNodeIndex = i;
                  presentQueueNodeIndex = i;
                  break;
              }
          }
      }

      if ( presentQueueNodeIndex == UINT32_MAX) {
          // If there's no queue that supports both present and graphics
          // try to find a separate present queue
          for ( u32 i = 0; i < queueCount; ++i ) {
              if ( supportsPresent[i] == VK_TRUE ) {
                  presentQueueNodeIndex = i;
                  break;
              }
          }
      }

      // Exit if either a graphics or a presenting queue hasn't been found
      if ( graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX) {
          ost::exitFatal("Could not find a graphics and/or presenting queue!", -1);
      }


      queueNodeIndex = graphicsQueueNodeIndex;

      // Get list of supported surface formats
      u32 formatCount;
      VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr));
      OST_ASSERT(formatCount > 0);

      std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
      VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice,
                                                           m_surface,
                                                           &formatCount,
                                                           surfaceFormats.data()));

      // If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
      // there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
      if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)) {
          colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
          colorSpace = surfaceFormats[0].colorSpace;
      } else {
          // iterate over the list of available surface format and
          // check for the presence of VK_FORMAT_B8G8R8A8_UNORM
          bool found_B8G8R8A8_UNORM = false;
          for ( auto &&surfaceFormat : surfaceFormats ) {
              if ( surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM ) {
                  colorFormat = surfaceFormat.format;
                  colorSpace = surfaceFormat.colorSpace;
                  found_B8G8R8A8_UNORM = true;
                  break;
              }
          }

          // in case VK_FORMAT_B8G8R8A8_UNORM is not available
          // select the first available color format
          if ( !found_B8G8R8A8_UNORM ) {
              colorFormat = surfaceFormats[0].format;
              colorSpace = surfaceFormats[0].colorSpace;
          }
      }

  }

  void VulkanSwapChain::connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device) noexcept {

      this->m_instance = instance;
      this->m_physicalDevice = physicalDevice;
      this->m_device = device;

  }

  void VulkanSwapChain::create(u32 *width, u32 *height, bool vsync) noexcept {

      VkSwapchainKHR oldSwapchain = swapChain;

      // Get physical device surface properties and formats
      VkSurfaceCapabilitiesKHR surfCaps;
      VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &surfCaps));

      // Get available present modes
      u32 presentModeCount;
      VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, NULL));
      OST_ASSERT(presentModeCount > 0);

      std::vector<VkPresentModeKHR> presentModes(presentModeCount);
      VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice,
                                                                m_surface,
                                                                &presentModeCount,
                                                                presentModes.data()));

      VkExtent2D swapchainExtent = {};
      // If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
      if ( surfCaps.currentExtent.width == ( u32 ) -1 ) {
          // If the surface size is undefined, the size is set to
          // the size of the images requested.
          swapchainExtent.width = *width;
          swapchainExtent.height = *height;
      } else {
          // If the surface size is defined, the swap chain size must match
          swapchainExtent = surfCaps.currentExtent;
          *width = surfCaps.currentExtent.width;
          *height = surfCaps.currentExtent.height;
      }


      // Select a present mode for the swapchain

      // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
      // This mode waits for the vertical blank ("v-sync")
      VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

      // If v-sync is not requested, try to find a mailbox mode
      // It's the lowest latency non-tearing present mode available
      if ( !vsync ) {

          for ( size_t i = 0; i < presentModeCount; i++ ) {
              if ( presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR ) {
                  swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                  break;
              }

              if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR)
                  && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
                  swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
              }
          }
      }

      // Determine the number of images
      u32 desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
      if ((surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCaps.maxImageCount)) {
          desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
      }

      // Find the transformation of the surface
      VkSurfaceTransformFlagsKHR preTransform;
      if ( surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) {
          // We prefer a non-rotated transform
          preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
      } else {
          preTransform = surfCaps.currentTransform;
      }

      // Find a supported composite alpha format (not all devices support alpha opaque)
      VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

      // Simply select the first composite alpha format available
      std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
          VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
          VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
          VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
          VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
      };

      for ( auto &compositeAlphaFlag : compositeAlphaFlags ) {
          if ( surfCaps.supportedCompositeAlpha & compositeAlphaFlag ) {
              compositeAlpha = compositeAlphaFlag;
              break;
          };
      }

      VkSwapchainCreateInfoKHR swapchainCI = {};
      swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      swapchainCI.pNext = nullptr;
      swapchainCI.surface = m_surface;
      swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
      swapchainCI.imageFormat = colorFormat;
      swapchainCI.imageColorSpace = colorSpace;
      swapchainCI.imageExtent = { swapchainExtent.width, swapchainExtent.height };
      swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
      swapchainCI.preTransform = ( VkSurfaceTransformFlagBitsKHR ) preTransform;
      swapchainCI.imageArrayLayers = 1;
      swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      swapchainCI.queueFamilyIndexCount = 0;
      swapchainCI.pQueueFamilyIndices = nullptr;
      swapchainCI.presentMode = swapchainPresentMode;
      swapchainCI.oldSwapchain = oldSwapchain;
      // Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
      swapchainCI.clipped = VK_TRUE;
      swapchainCI.compositeAlpha = compositeAlpha;

      // Set additional usage flag for blitting from the swapchain images if supported
      VkFormatProperties formatProps;
      vkGetPhysicalDeviceFormatProperties(m_physicalDevice, colorFormat, &formatProps);
      if ((formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR)
          || (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT)) {
          swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
      }

      VK_CHECK_RESULT(vkCreateSwapchainKHR(m_device, &swapchainCI, nullptr, &swapChain));

      // If an existing swap chain is re-created, destroy the old swap chain
      // This also cleans up all the presentable images
      if ( oldSwapchain != VK_NULL_HANDLE ) {
          for ( u32 i = 0; i < imageCount; i++ ) {
              vkDestroyImageView(m_device, buffers[i].view, nullptr);
          }
          vkDestroySwapchainKHR(m_device, oldSwapchain, nullptr);
      }
      VK_CHECK_RESULT(vkGetSwapchainImagesKHR(m_device, swapChain, &imageCount, nullptr));

      // Get the swap chain images
      images.resize(imageCount);
      VK_CHECK_RESULT(vkGetSwapchainImagesKHR(m_device, swapChain, &imageCount, images.data()));

      // Get the swap chain buffers containing the image and imageview
      buffers.resize(imageCount);

      for ( u32 i = 0; i < imageCount; i++ ) {
          VkImageViewCreateInfo colorAttachmentView = {};
          colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
          colorAttachmentView.pNext = nullptr;
          colorAttachmentView.format = colorFormat;
          colorAttachmentView.components = {
              VK_COMPONENT_SWIZZLE_R,
              VK_COMPONENT_SWIZZLE_G,
              VK_COMPONENT_SWIZZLE_B,
              VK_COMPONENT_SWIZZLE_A
          };
          colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
          colorAttachmentView.subresourceRange.baseMipLevel = 0;
          colorAttachmentView.subresourceRange.levelCount = 1;
          colorAttachmentView.subresourceRange.baseArrayLayer = 0;
          colorAttachmentView.subresourceRange.layerCount = 1;
          colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
          colorAttachmentView.flags = 0;

          buffers[i].image = images[i];

          colorAttachmentView.image = buffers[i].image;

          VK_CHECK_RESULT(vkCreateImageView(m_device, &colorAttachmentView, nullptr, &buffers[i].view));
      }

  }

  VkResult VulkanSwapChain::acquireNextImage(VkSemaphore presentCompleteSemaphore, u32 *imageIndex) noexcept {

      // By setting timeout to UINT64_MAX we will always wait until the next image has been acquired or an actual error is thrown
      // With that we don't have to handle VK_NOT_READY
      return vkAcquireNextImageKHR(m_device,
                                   swapChain,
                                   UINT64_MAX,
                                   presentCompleteSemaphore,
                                   ( VkFence ) nullptr,
                                   imageIndex);

  }

  VkResult VulkanSwapChain::queuePresent(VkQueue queue, u32 imageIndex, VkSemaphore waitSemaphore) noexcept {

      VkPresentInfoKHR presentInfo = {};
      presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
      presentInfo.pNext = nullptr;
      presentInfo.swapchainCount = 1;
      presentInfo.pSwapchains = &swapChain;
      presentInfo.pImageIndices = &imageIndex;

      // Check if a wait semaphore has been specified to wait for before presenting the image
      if ( waitSemaphore != VK_NULL_HANDLE ) {
          presentInfo.pWaitSemaphores = &waitSemaphore;
          presentInfo.waitSemaphoreCount = 1;
      }

      return vkQueuePresentKHR(queue, &presentInfo);

  }

  void VulkanSwapChain::destroy() noexcept {

      if ( swapChain != VK_NULL_HANDLE ) {
          for ( u32 i = 0; i < imageCount; i++ ) {
              vkDestroyImageView(m_device, buffers[i].view, nullptr);
          }
      }

      if ( m_surface != VK_NULL_HANDLE ) {
          vkDestroySwapchainKHR(m_device, swapChain, nullptr);
          vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
      }

      m_surface = VK_NULL_HANDLE;
      swapChain = VK_NULL_HANDLE;
  }

}