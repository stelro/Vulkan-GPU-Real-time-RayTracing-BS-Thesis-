/* =======================================================================
   $File: VulkanTexture.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#pragma once

#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#include "vulkan/vulkan.h"

// #include <gli/gli.hpp>

#include "VulkanTools.hh"
#include "VulkanDevice.hh"
#include "VulkanBuffer.hh"

namespace ost {

  class Texture {
  public:

    ost::VulkanDevice*          device;
    VkImage                     image;
    VkImageLayout               imageLayout;
    VkDeviceMemory              deviceMemory;
    VkImageView                 view;
    u32                         width;
    u32                         height;
    u32                         mipLevels;
    u32                         layerCount;
    VkDescriptorImageInfo       descriptor;

    // Optional sampler to use with this texture
    VkSampler                   sampler;

    // Update image descriptor from current sampler, view and image layout
    void updateDescriptor() {

      descriptor.sampler = sampler;
      descriptor.imageView = view;
      descriptor.imageLayout = imageLayout;
    }

    // Release all Vulkan resources held by this texture
    void destroy() {

      vkDestroyImageView(device->logicalDevice, view, nullptr);
      vkDestroyImage(device->logicalDevice, image, nullptr);
      if ( sampler ) {
        vkDestroySampler(device->logicalDevice, sampler, nullptr);
      }
      vkFreeMemory(device->logicalDevice, deviceMemory, nullptr);
    }
  };

} // namespace ost
