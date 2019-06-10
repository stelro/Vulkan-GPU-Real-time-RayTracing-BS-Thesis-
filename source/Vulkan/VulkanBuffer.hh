/* =======================================================================
   $File: VulkanBuffer.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#pragma once

//Engine includes
#include "vulkan/vulkan.h"
#include "VulkanTools.hh"
#include "../core.hh"

//C++ includes
#include <vector>

namespace ost {

  // VulkanBuffer object wrapper of VkBuffer
  // To be filled by an external source like the VulkanDevice
  struct VulkanBuffer {

      // Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
      VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept {
          return vkMapMemory(device, memory, offset, size, 0, &mapped);
      }

      // Unmap a mapped memory range
      void unmap() {
          if ( mapped ) {
              vkUnmapMemory(device, memory);
              mapped = nullptr;
          }
      }

      // Attach the allocated memory block to the buffer
      VkResult bind(VkDeviceSize offset = 0) noexcept {
          return vkBindBufferMemory(device, buffer, memory, offset);
      }

      // Setup the default descriptor for this buffer
      void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept {
          descriptor.offset = offset;
          descriptor.buffer = buffer;
          descriptor.range = size;
      }

      // Copies the specified data to the mapped buffer
      void copyTo(void *data, VkDeviceSize size) noexcept {
          OST_ASSERT(mapped);
          memcpy(mapped, data, size);
      }

      // Flush a memory range of the buffer to make it visible to the device
      VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept {
          VkMappedMemoryRange mappedRange = {};
          mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
          mappedRange.memory = memory;
          mappedRange.offset = offset;
          mappedRange.size = size;
          return vkFlushMappedMemoryRanges(device, 1, &mappedRange);
      }

      // Invalidate a memory range of the buffer to make it visible to the host
      VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept {
          VkMappedMemoryRange mappedRange = {};
          mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
          mappedRange.memory = memory;
          mappedRange.offset = offset;
          mappedRange.size = size;
          return vkInvalidateMappedMemoryRanges(device, 1, &mappedRange);
      }

      // Release all Vulkan resources held by this buffer
      void destroy() noexcept {

          if ( buffer ) {
              vkDestroyBuffer(device, buffer, nullptr);
          }

          if ( memory ) {
              vkFreeMemory(device, memory, nullptr);
          }
      }

      VkDevice                          device          = VK_NULL_HANDLE;
      VkBuffer                          buffer          = VK_NULL_HANDLE;
      VkDeviceMemory                    memory          = VK_NULL_HANDLE;
      VkDescriptorBufferInfo            descriptor;
      VkDeviceSize                      size            = 0;
      VkDeviceSize                      alignment       = 0;
      void*                             mapped          = nullptr;

      VkBufferUsageFlags                usageFlags;
      VkMemoryPropertyFlags             memoryPropertyFlags;
  };
}